#include "RpcApi.h"

namespace media {

SDKServer::SDKServer(int port)
{
    thread_ = new std::thread([&]() {
        srv_.reset(new rpc::server(port));
        std::function<void(float)> fuc = std::bind(&SDKServer::onProgress, this, std::placeholders::_1);
        srv_->bind("progress", fuc);

        std::function<int(int)> fuc2 = std::bind(&SDKServer::onState, this, std::placeholders::_1);
        srv_->bind("state", fuc2);

        fprintf(stderr, "start rpc srv!\n");
        srv_->run();
    });
}

SDKServer::~SDKServer()
{
    stopServer();
}

int SDKServer::stopServer()
{
    srv_->stop();
    thread_->join();
    return 0;
}

void SDKServer::onProgress(float v)
{
    fprintf(stderr, "%s - %d %f\n", __FUNCTION__, __LINE__, v);
}

int SDKServer::onState(int state)
{
    fprintf(stderr, "%s - %d state:%d\n", __FUNCTION__, __LINE__, state);
    return 0;
}

SDKClient::SDKClient(int port)
{
    client_.reset(new rpc::client("127.0.0.1", port));
    port_ = port;
}

SDKClient::~SDKClient()
{}

void SDKClient::stop()
{
    if (is_connect())
        client_->call("stop");
}

bool SDKClient::is_connect()
{
    auto state = client_->get_connection_state();
    if (state == rpc::client::connection_state::connected)
        client_.reset(new rpc::client("127.0.0.1", port_));

    state = client_->get_connection_state();
    if (state == rpc::client::connection_state::connected)
        return true;
    return false;
}

AppServer::AppServer(int port)
{
    thread_ = new std::thread([&]() {
        srv_.reset(new rpc::server(port));

        std::function<void()> fuc2 = std::bind(&AppServer::onStop, this);
        srv_->bind("stop", fuc2);

        fprintf(stderr, "start rpc srv!\n");
        srv_->run();
    });
}

AppServer::~AppServer()
{
    stopServer();
}

int AppServer::stopServer()
{
    srv_->stop();
    thread_->join();
    return 0;
}

bool AppServer::isQuit()
{
    return bQuit_;
}

void AppServer::onStop()
{
    fprintf(stderr, "onStop\n");
    bQuit_ = true;
}

AppClient::AppClient(int port)
{
    client_.reset(new rpc::client("127.0.0.1", port));
    port_ = port;
}

AppClient::~AppClient()
{}

void AppClient::progress(float v)
{
    if (is_connect())
        client_->call("progress", v);
}

int AppClient::state(int s)
{
    if (is_connect())
        client_->call("state", s);
    return 0;
}

bool AppClient::is_connect()
{
    auto state = client_->get_connection_state();
    if (state == rpc::client::connection_state::connected)
        client_.reset(new rpc::client("127.0.0.1", port_));

    state = client_->get_connection_state();
    if (state == rpc::client::connection_state::connected)
        return true;
    return false;
}

static AppServer *g_srv = nullptr;
void setGlobalAppServer(AppServer *srv)
{
    g_srv = srv;
}

AppServer *getGlobalAppServer()
{
    return g_srv;
}

static AppClient *g_cli = nullptr;
void setGlobalAppClient(AppClient *cli)
{
    g_cli = cli;
}

AppClient *getGlobalAppClient()
{
    return g_cli;
}
}

media::AppClient g_client;
media::AppServer g_server;

extern "C" {
void crpc_server_start()
{}

void crpc_server_stop()
{

}

bool crpc_server_isQuit()
{
	return g_server.isQuit();
}

void crpc_client_start()
{}


void crpc_client_stop()
{}

void crpc_client_progress(float v)
{
	g_client.progress(v);
}

int crpc_client_state(int s)
{
	return g_client.state(s);
}

std::string g_model_path;
int crpc_set_model_path(char *path)
{
	g_model_path = path;
	return 0;
}

const char* crpc_get_model_path()
{
	return g_model_path.data();
}

bool mMoveFile(char* src, char* dst) {
	std::string sourceFilePath(src);
	std::string destinationFilePath(dst);
	// 将源文件路径和目标文件路径从多字节字符串转换为宽字符字符串
	int sourceFilePathLength = MultiByteToWideChar(CP_UTF8, 0, sourceFilePath.c_str(), -1, nullptr, 0);
	int destinationFilePathLength = MultiByteToWideChar(CP_UTF8, 0, destinationFilePath.c_str(), -1, nullptr, 0);
	std::wstring sourceFilePathW(sourceFilePathLength, L'\0');
	std::wstring destinationFilePathW(destinationFilePathLength, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, sourceFilePath.c_str(), -1, &sourceFilePathW[0], sourceFilePathLength);
	MultiByteToWideChar(CP_UTF8, 0, destinationFilePath.c_str(), -1, &destinationFilePathW[0], destinationFilePathLength);

	if (MoveFileW(sourceFilePathW.c_str(), destinationFilePathW.c_str())) {
		return true;
	}
	else {
		return false;
	}
}

}