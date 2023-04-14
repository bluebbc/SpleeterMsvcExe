#include "RpcApi.h"

media::AppClient g_client(8090);
media::AppServer g_server(8091);

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
std::string g_dst_voice;
int crpc_set_dst_voice(char *path)
{
	g_dst_voice = path;
	return 0;
}

const char* crpc_get_dst_voice()
{
	return g_dst_voice.data();
}
std::string g_dst_music;
int crpc_set_dst_music(char *path)
{
	g_dst_music = path;
	return 0;
}

const char* crpc_get_dst_music()
{
	return g_dst_music.data();
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

	DeleteFileW(destinationFilePathW.c_str());
	if (MoveFileW(sourceFilePathW.c_str(), destinationFilePathW.c_str())) {
		return true;
	}
	else {
		return false;
	}
}

void moveFileAuto(char *src)
{
	std::string s(src);
	if (s.find("vocals") != std::string::npos)
	{
		mMoveFile(src, (char*)crpc_get_dst_voice());
	}
	else {
		mMoveFile(src, (char*)crpc_get_dst_music());
	}
}

}