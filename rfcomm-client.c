#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int s, status, bytes_read;
    char dest[18] = "AC:D1:B8:D1:D9:34";
    char buf[1024] = { 0 };
    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
    if( status == 0 ) {
        status = write(s, "HelloWorld", sizeof("HelloWorld"));
    }

    if( status < 0 ) perror("HelloWorld");

    while(1)
    {
        bytes_read = read(s, buf, sizeof(buf));
	if( bytes_read > 0 ) {
        printf("Received [%s] after SendData\n", buf);
	}
        if(!(strcmp(buf, "SendData")))
        {
            status = write(s, "SensorData", sizeof("SensorData"));
            if( status < 0 ) perror("SensorData");
        }
    }
    close(s);
    return 0;
}
