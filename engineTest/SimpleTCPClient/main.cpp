#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

int main(int argc, char* argv[]){
	//���ַ�����IP��ַת��Ϊu_long
	unsigned long ip = inet_addr("127.0.0.1");
	//�Ѷ˿ں�ת��������
	short port = 8000;
	printf("Connecting to %s:%d....../n",inet_ntoa(*(in_addr*)&ip),port);
	WSADATA wsa;
	//��ʼ���׽���DLL
	if(WSAStartup(MAKEWORD(2,2),&wsa)!=0){
		printf("�׽��ֳ�ʼ��ʧ��!");
		exit(-1);
	}
	//�����׽���
	SOCKET sock;
	if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==INVALID_SOCKET){
		printf("�����׽���ʧ�ܣ�");
		exit(-1);
	}
	int size = 0;
	int len = 4;
	if( 0 != ::getsockopt( sock, SOL_SOCKET, SO_SNDBUF, (char*)&size, &len ) )
	{
		return 0;
	}
	struct sockaddr_in serverAddress;
	memset(&serverAddress,0,sizeof(sockaddr_in));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.S_un.S_addr = ip;
	serverAddress.sin_port = htons(port);
	//�����ͷ�����������
	if(connect(sock,(sockaddr*)&serverAddress,sizeof(serverAddress))==SOCKET_ERROR){
		printf("��������ʧ�ܣ�");
		exit(-1);
	}
	printf( "��������" );
	char buf[256] = { 0 };
	send(sock,buf,sizeof(buf),0);
	shutdown( sock, SD_SEND );
	Sleep( ~0 );
	//�����׽���ռ�õ���Դ
	WSACleanup();
	return 0;
}