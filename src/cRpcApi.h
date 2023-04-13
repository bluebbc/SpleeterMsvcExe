#pragma once


void crpc_server_start();
void crpc_server_stop();
bool crpc_server_isQuit();

void crpc_client_start();
void crpc_client_stop();
void crpc_client_progress(float v);
int crpc_client_state(int s);

int crpc_set_dst_voice(char *path);
const char* crpc_get_dst_voice();

int crpc_set_dst_music(char *path);
const char* crpc_get_dst_music();

int crpc_set_model_path(char *path);
const char* crpc_get_model_path();

bool mMoveFile(char* src, char* dst);
void moveFileAuto(char *src);
