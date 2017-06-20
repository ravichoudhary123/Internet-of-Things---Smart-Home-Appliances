#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>


int s, client = 0;

void GetSensor1Data()
{
    char buf[1024] = { 0 };

    printf("In %s\n",__FUNCTION__);

    int status = 0 , bytes_read = 0;
    status = write(client, "SendData", sizeof("SendData"));
    if (status == -1)
    {
         perror("Write server");
    }

    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
    printf("Received [%s] after Send Data\n", buf);
    }
}

void GetSensor2Data()
{
    
}


static void
on_key_press (GtkWidget* widget, GdkEventKey * event, gpointer data)
{
    switch(event->keyval)
    {
        case GDK_Escape:
            gtk_widget_destroy(widget);
            gtk_main_quit();
            break;
        default:
            //printf("Invalid key\n");
        break;
    }

}


int main(int argc, char **argv)
{
//Server realted variables
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int  bytes_read = 0;
    socklen_t opt = sizeof(rem_addr);

//GUI related variables
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *Sensor1_box;
    GtkWidget *quit_box;
    GtkWidget *Sensor1_button;
    //GtkWidget *Sensor1_label;
    GtkWidget *Sensor2_button;
    //GtkWidget *Sensor2_label;
    GtkWidget *quit_button;
    //GtkWidget *quit_label;

    GdkColor color;

    gtk_init (NULL, NULL);

    color.red = 0x0000;
    color.green = 0xBF00; //0x0000;
    color.blue = 0xFF00 ;//0x0000;

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_resize((GtkWindow *)window,800,600);
    gtk_window_set_title((GtkWindow *)window, "WSN");

    gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);
    gtk_window_set_decorated((GtkWindow *)window, FALSE);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
                        GTK_SIGNAL_FUNC (gtk_main_quit), NULL);
    g_signal_connect (G_OBJECT(window), "key-press-event",
                      G_CALLBACK(on_key_press), (gpointer) NULL);

    main_box = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), main_box);
    gtk_widget_show (main_box);

    gtk_widget_modify_fg (window, GTK_STATE_NORMAL, &color);

    Sensor1_box = gtk_hbox_new (FALSE, 0);

    Sensor1_button = gtk_button_new_with_label("Sensor1");
    gtk_signal_connect_object (GTK_OBJECT (Sensor1_button), "clicked",
                               GTK_SIGNAL_FUNC (GetSensor1Data),
                               GTK_OBJECT (window));
    gtk_box_pack_start (GTK_BOX (Sensor1_box), Sensor1_button, FALSE, FALSE, 75);
    gtk_widget_set_size_request(Sensor1_button, 500, 100);

    Sensor2_button = gtk_button_new_with_label("Sensor2");
    gtk_signal_connect_object (GTK_OBJECT (Sensor1_button), "clicked",
                               GTK_SIGNAL_FUNC (GetSensor2Data),
                               GTK_OBJECT (window));
    gtk_box_pack_start (GTK_BOX (Sensor1_box), Sensor2_button, FALSE, FALSE, 75);
    gtk_widget_set_size_request(Sensor2_button, 500, 100);

    gtk_box_pack_start (GTK_BOX (main_box), Sensor1_box, FALSE, FALSE, 10);
    gtk_widget_show (Sensor1_button);
    gtk_widget_show (Sensor2_button);
    gtk_widget_show (Sensor1_box);

    quit_box = gtk_hbox_new (FALSE, 0);

    quit_button = gtk_button_new_with_label("Quit");
    gtk_signal_connect_object (GTK_OBJECT (quit_button), "clicked",
                               GTK_SIGNAL_FUNC (gtk_main_quit),
                               GTK_OBJECT (window));
    gtk_box_pack_start (GTK_BOX (quit_box), quit_button, FALSE, FALSE, 75);
    gtk_widget_set_size_request(quit_button, 500, 100);

    gtk_box_pack_start (GTK_BOX (main_box), quit_box, FALSE, FALSE, 10);
    gtk_widget_show (quit_button);
    gtk_widget_show (quit_box);


    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }

    gtk_widget_show (main_box);
    gtk_window_set_decorated((GtkWindow *)window, FALSE);
    gtk_widget_show (window);
    gtk_main ();

#if 0
    while(1)
    {
        status = write(client, "SendData", sizeof("SendData"));
	if (status == -1)
        {
             perror("Write server");
        }
        bytes_read = read(client, buf, sizeof(buf));
        if( bytes_read > 0 ) {
        printf("Received [%s] after Send Data\n", buf);
        }
    }
 
#endif
    // close connection
    close(client);
    close(s);
    return 0;
}

