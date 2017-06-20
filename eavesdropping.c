#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char **argv)
{
/*Structure to store the found devices*/
    inquiry_info *devices = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };
    struct sockaddr_rc address = { 0 };
    int s, status;
    char dest[19] = { 0 };

/*Retrieve the resource number of the first available Bluetooth Adapter*/
    dev_id = hci_get_route(NULL);
    if (dev_id < 0)
        {
            perror("Bluetooth Adapter not found");
            exit(1);
        }

/*Allocate the resources to be used by the Bluetooth Adapter for scanning nearby devices
Open a socket connection to the microcontroller on the local Bluetooth adapter*/
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

/*Inquiry time = 8*1.24*/
    len  = 8;
/*Maximum of 255 devices to discover*/
    max_rsp = 255;
/*Don't use data of previous enquiry*/
    flags = IREQ_CACHE_FLUSH;

    devices = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

/*Perform Bluetooth Device Discovery and return list of detected devices*/    
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &devices, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");

    for (i = 0; i < num_rsp; i++) 
    {
        ba2str(&(devices+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
/*Get the device friendly name*/
        if (hci_read_remote_name(sock, &(devices+i)->bdaddr, sizeof(name), 
            name, 0) < 0)
/*If friendly name is not found*/
        strcpy(name, "[unknown]");

        printf("%s  %s\n", addr, name);

/*This is the server we want to connect to*/	
	if(!(strcmp (name , "ubuntu-0")))
	{
		printf("Server found %s\n",name);	
        	ba2str(&(devices+i)->bdaddr, dest);
		break;
			
	}
	 
    }

    if(dest)
    {
        
        // allocate a socket
        s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

        // set the connection parameters (who to connect to)
        address.rc_family = AF_BLUETOOTH;
        address.rc_channel = (uint8_t) 1;
        str2ba( dest, &address.rc_bdaddr );

        // connect to server
        status = connect(s, (struct sockaddr *)&address, sizeof(address));

        // send a message
        if( status == 0 ) 
        {
            status = write(s, "Hello-Raspberry PI", sizeof("Hello-Raspberry PI"));
        }

        if( status < 0 ) perror("Unable to send data to server");

        close(s);
    }    	

     
    free( devices );
    close( sock );
    return 0;
}

