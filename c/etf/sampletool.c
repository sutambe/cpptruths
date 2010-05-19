#include <gnome.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <gtk/gtk.h>
#include <unistd.h>

#include "linkdef.h"
#include "redirect.h"
#include "registrar.h"
#include "message_router.h"

static int patternid;
static Router *rtr;
static Registrar *reg;

static int rd_handle;
static LinkDef *linkdef_ptr;

void del_message_func(Message *m);
void *gui(void *arg);

int reply_receive (Message *m, char *pattern)
{
    if (strstr(pattern,"REPLY_SYM2SYM"))
    {
        //printf("Sampletool reply_receive: GOT %s\n",pattern);
    }

    return 0; // SUCCESS 
}

void notification_receive(Message *m, char *pattern)
{
    gchar *text[4];
    static int once=0;
    extern GtkWidget *linkdef_list;
    extern GtkWidget *input_pat_list;
    extern GtkWidget *output_pat_list;
    extern int gui_init;

    if (strstr(pattern,"REDIRECT_DEFINITION"))
    {
        if(gui_init)
        {
            text[0] = (gchar *) m->data[3];
            text[1] = (gchar *) m->data[4];
            text[2] = (gchar *) m->data[0];
            text[3] = (gchar *) m->data[1];
        
            gtk_clist_append (GTK_CLIST(linkdef_list), text); 
        }
    }
    if (0 == once)
    {
        PatternListIterator iterator;
        MessagePattern mp;
        int i;
        
        for (i=0;i <=1; i++)
        {
            iterator = pattern_itr_begin(reg, i );
            while (pattern_itr_at(iterator, &mp))
            {
                if(gui_init)
                {
                    switch (mp.type)
                    {
                        case REQUEST:
                            text[0] = (gchar *) "REQUEST";
                            break;
                        case REPLY:
                            text[0] = (gchar *) "REPLY";
                            break;
                        case NOTIFICATION:
                            text[0] = (gchar *) "NOTIFICATION";
                            break;
                    }
                    text[1] = (gchar *) mp.pattern;
                    if (0 == i)
                        gtk_clist_append (GTK_CLIST(input_pat_list), text); 
                    else 
                        gtk_clist_append (GTK_CLIST(output_pat_list), text); 
                }
                iterator = pattern_itr_next(iterator);
            }
        }
        once=1;
    }
}

int tool_init(Registrar *r,int handle,void *master_dataptr)
{
    pthread_t guithread;
    int threadret;
    char *message1 = "Thread 1";
    
    rd_handle=handle;
    linkdef_ptr=(LinkDef *)master_dataptr;

    rtr=r->router_ptr;
    reg=r;    
    printf("sampletool.c: Calling pthread\n");
    threadret=pthread_create(&guithread,NULL,gui,message1);
    pthread_detach(guithread);
    sleep(1);
    if(threadret)
    {
        fprintf(stderr,"sampletool.c: Thread can't be created.\n");
    }
    else
    {
        printf("sampletool.c: Thread created.\n");
        register_input_pattern(r,rd_handle,"REDIRECT_DEFINITION",NOTIFICATION);
        patternid=register_output_pattern(r,handle,"REQUEST_SYM2SYM",REQUEST);
    }
    return TOOLINIT_SUCCESS;
}

void _init(void)
{
    //printf("_init: sampletool\n");
}
void _fini(void)
{
/*  There is a problem here. ED's fini is called before tool's _fini.
    So tool can not call any ED API.  This does not seem to be a right
    thing.
*/
/*    set_ED_IsActiveHook(rd_handle,0);
    set_ED_LookupHook(rd_handle,0);
    set_ED_OffsetHook(rd_handle,0);
    set_ED_DefinitionHook(rd_handle,0);
    set_ED_SymdefHook(rd_handle,0);*/
    //printf("_fini: sampletool\n");
}
/*
void *gui(void *arg)
{
    while(1)
    {
        printf("Inside thread: %s\n",(char *)arg);
        sleep(1);
    }
    return 0;
}
*/


static void redirect( GtkWidget *widget, gpointer   data );
static void sort_table( GtkWidget *widget, gpointer   data );
static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data );
static void win_destroy( GtkWidget *widget,
                     gpointer   data );
//static void common_callback( GtkWidget *widget, gpointer   data );
//static void messagebox_clicked(GnomeDialog *dlg,gint button,gpointer data);
                             
GtkWidget *text_linkfname, *text_linklibname;
GtkWidget *text_deffname, *text_deflibname;
GtkWidget *dlg;
GtkWidget *linkdef_list;
GtkWidget *input_pat_list;
GtkWidget *output_pat_list;
int gui_init=0;

void *gui(void *arg)
{
    /* GtkWidget is the storage type for widgets */
    GtkWidget *window;
    GtkWidget *label;
    GtkWidget *button, *exitbutton;
    GtkWidget *vbox, *vbox1, *vbox2;
    GtkWidget *hbox;
    GtkWidget *notebook;
    GtkWidget *frame;
    GtkWidget *scrolled_window;
    //GSList *group = NULL;
    gboolean expand = FALSE;
    gboolean fill = TRUE;
    gboolean homogeneous = FALSE;
    gint padding = 2;
    gint spacing = 2;
    gchar *titles[4] = {"Link fname", "Caller", "Wrapper", "Wrapper Library"};
    gchar *input_pat_titles[2] = { "Type", "Input Patterns" };
    gchar *output_pat_titles[2] = { "Type", "Output Patterns" };
    
    /* This is called in all GTK applications. Arguments are parsed
     * from the command line and are returned to the application. */
    int argc=1;
    char *argument_vector[]= {"Sampletool",0};
    char **argv=argument_vector;
    
    gtk_init (&argc, &argv);
    
    /* create a new window */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    /* When the window is given the "delete_event" signal (this is given
     * by the window manager, usually by the "close" option, or on the
     * titlebar), we ask it to call the delete_event () function
     * as defined above. The data passed to the callback
     * function is NULL and is ignored in the callback function. */
    g_signal_connect (G_OBJECT (window), "delete_event",
		      G_CALLBACK (delete_event), NULL);
    
    /* Here we connect the "destroy" event to a signal handler.  
     * This event occurs when we call gtk_widget_destroy() on the window,
     * or if we return FALSE in the "delete_event" callback. */
    g_signal_connect (G_OBJECT (window), "destroy",
		      G_CALLBACK (win_destroy), NULL);
    
    /* Sets the border width of the window. */
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    
    //gnome_app_create_menus(GNOME_APP(window),menubar);
    //gnome_app_create_toolbar(GNOME_APP(window),toolbar);
    
    vbox  = gtk_vbox_new(homogeneous,spacing);
    vbox1 = gtk_vbox_new(homogeneous,spacing);
    vbox2 = gtk_vbox_new(homogeneous,spacing);
    hbox  = gtk_hbox_new(homogeneous,spacing);
    
    label = gtk_label_new("LINK:");
    gtk_box_pack_start(GTK_BOX(vbox1),label,expand,fill,padding);
    
    label = gtk_label_new("Function Name:");
    gtk_box_pack_start(GTK_BOX(vbox1),label,expand,fill,padding);
    
    text_linkfname = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox1),text_linkfname,expand,fill,padding);
    
    label = gtk_label_new("Caller:");
    gtk_box_pack_start(GTK_BOX(vbox1),label,expand,fill,padding);
    
    text_linklibname = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox1),text_linklibname,expand,fill,padding);

    
        
    label = gtk_label_new("DEFINITION:");
    gtk_box_pack_start(GTK_BOX(vbox2),label,expand,fill,padding);
    
    label = gtk_label_new("Wrapper Name:");
    gtk_box_pack_start(GTK_BOX(vbox2),label,expand,fill,padding);
    
    text_deffname = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox2),text_deffname,expand,fill,padding);
    
    label = gtk_label_new("Wrapper Library:");
    gtk_box_pack_start(GTK_BOX(vbox2),label,expand,fill,padding);
    
    text_deflibname = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox2),text_deflibname,expand,fill,padding);

    gtk_container_add(GTK_CONTAINER(hbox),vbox1);
    gtk_container_add(GTK_CONTAINER(hbox),vbox2);
    gtk_container_add(GTK_CONTAINER(vbox),hbox);
    
    /* Creates a new button with the label "Hello World". */
    button = gtk_button_new_with_label ("Redirect");
        
    /* When the button receives the "clicked" signal, it will call the
     * function hello() passing it NULL as its argument.  The hello()
     * function is defined above. */
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (redirect), NULL);
    
    gtk_box_pack_start(GTK_BOX(vbox),button,expand,fill,padding);

    exitbutton = gtk_button_new_from_stock (GTK_STOCK_QUIT);
    g_signal_connect_swapped (G_OBJECT (exitbutton), "clicked",
                              G_CALLBACK (gtk_widget_destroy),
                              G_OBJECT (window));

                                
    gtk_box_pack_start(GTK_BOX(vbox),exitbutton,expand,fill,padding);

    notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook),GTK_POS_TOP);
    
    frame = gtk_frame_new ("Redirect");
	gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
	//gtk_widget_set_size_request (frame, 500, 375);
		
	gtk_container_add (GTK_CONTAINER (frame), vbox);
	
	label = gtk_label_new ("Redirect");
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);

    frame = gtk_frame_new ("Table");
	gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
	//gtk_widget_set_size_request (frame, 500, 375);
		
	//gtk_container_add (GTK_CONTAINER (frame), vbox);
	
    linkdef_list = gtk_clist_new_with_titles (4, titles);
		
    gtk_clist_set_column_width(GTK_CLIST(linkdef_list), 0, 200); /* set sensible column widths */
    gtk_clist_set_column_width(GTK_CLIST(linkdef_list), 1, 400);
    gtk_clist_set_column_width(GTK_CLIST(linkdef_list), 2, 200);
    
    gtk_clist_set_sort_type(GTK_CLIST(linkdef_list), GTK_SORT_ASCENDING);
    gtk_clist_set_sort_column(GTK_CLIST(linkdef_list), 0);
    //Auto sorting makes it really slow
    //gtk_clist_set_auto_sort(GTK_CLIST(linkdef_list), TRUE);
    gtk_clist_column_titles_active(GTK_CLIST(linkdef_list));
    g_signal_connect (G_OBJECT (linkdef_list), "click-column",
                      G_CALLBACK (sort_table), NULL);
    
    gtk_clist_set_column_justification (GTK_CLIST(linkdef_list), 0, GTK_JUSTIFY_CENTER); /* justify leftmost column */

    scrolled_window = gtk_scrolled_window_new (NULL, NULL); /* create scrolled window widgets to place the clists in */
	
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
				    GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC); /* select scrollbar policy to show          */
   
    gtk_container_add(GTK_CONTAINER(scrolled_window), linkdef_list);
    gtk_container_add(GTK_CONTAINER(frame), scrolled_window);
	
	label = gtk_label_new ("Table");
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);

///********************************************************************************
    
    frame = gtk_frame_new ("Input Patterns");
	gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
	//gtk_widget_set_size_request (frame, 500, 375);
		
	//gtk_container_add (GTK_CONTAINER (frame), vbox);
	
    input_pat_list = gtk_clist_new_with_titles (2, input_pat_titles);
		
    gtk_clist_set_column_width(GTK_CLIST(input_pat_list), 0, 200); /* set sensible column widths */
    gtk_clist_set_column_width(GTK_CLIST(input_pat_list), 1, 400);
    
    gtk_clist_set_column_justification (GTK_CLIST(input_pat_list), 0, GTK_JUSTIFY_CENTER); /* justify leftmost column */
    
    scrolled_window = gtk_scrolled_window_new (NULL, NULL); /* create scrolled window widgets to place the clists in */
	
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
				    GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC); /* select scrollbar policy to show          */
   
    gtk_container_add(GTK_CONTAINER(scrolled_window), input_pat_list);
    gtk_container_add(GTK_CONTAINER(frame), scrolled_window);
	
	label = gtk_label_new ("Input Patterns");
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    
    
// ******************************************************************************** 
    
    frame = gtk_frame_new ("Output Patterns");
	gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
	//gtk_widget_set_size_request (frame, 500, 375);
		
	//gtk_container_add (GTK_CONTAINER (frame), vbox);
	
    output_pat_list = gtk_clist_new_with_titles (2, output_pat_titles);
		
    gtk_clist_set_column_width(GTK_CLIST(output_pat_list), 0, 200); /* set sensible column widths */
    gtk_clist_set_column_width(GTK_CLIST(output_pat_list), 1, 400);
    
    gtk_clist_set_column_justification (GTK_CLIST(output_pat_list), 0, GTK_JUSTIFY_CENTER); /* justify leftmost column */
    
    scrolled_window = gtk_scrolled_window_new (NULL, NULL); /* create scrolled window widgets to place the clists in */
	
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
				    GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC); /* select scrollbar policy to show          */
   
    gtk_container_add(GTK_CONTAINER(scrolled_window), output_pat_list);
    gtk_container_add(GTK_CONTAINER(frame), scrolled_window);
	
	label = gtk_label_new ("Output Patterns");
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);

    
    
    
    
    
       
    gtk_container_add (GTK_CONTAINER (window), notebook);


            
    /* This packs the button into the window (a gtk container). */
    //gtk_container_add (GTK_CONTAINER (window), preferencebox);
    
    /* The final step is to display this newly created widget. */
    //gtk_widget_show (vbox);
    
    /* and the window */
    gtk_widget_show_all (window);
    gui_init=1;
    /* All GTK applications must have a gtk_main(). Control ends here
     * and waits for an event to occur (like a key press or
     * mouse event). */
    gtk_main ();
    gui_init=0;
    
    return 0;
}


/* This is a callback function. The data arguments are ignored
 * in this example. More on callbacks below. */
static void redirect( GtkWidget *widget,
                      gpointer   data )
{
    Message message;
    char buffer[2000]={0};
    const char *linkfname, *linklibname, *deffname, *deflibname;
    
    linkfname=gtk_entry_get_text(GTK_ENTRY(text_linkfname));
    linklibname=gtk_entry_get_text(GTK_ENTRY(text_linklibname));
    deffname=gtk_entry_get_text(GTK_ENTRY(text_deffname));
    deflibname=gtk_entry_get_text(GTK_ENTRY(text_deflibname));
    
    if((0 != strlen(linkfname)) &&
       (0 != strlen(linklibname)) &&
       (0 != strlen(deffname)) &&
       (0 != strlen(deflibname)))
    {
        sprintf(buffer,"%s,%s : %s,%s\n",linkfname, linklibname, deffname, deflibname);
        g_print(buffer);
        
        new_request(rtr, &message, rd_handle, SUCCESS, SYNC_MODE,
                        patternid, del_message_func);
        message.data_entries=4;
        message.data[0]=(void *)linkfname;
        message.data[1]=(void *)linklibname;
        message.data[2]=(void *)deffname;
        message.data[3]=(void *)deflibname;
        send_message(rtr, &message);
    }
    else
    {
        g_print("Length of string is equal to zero.\n");
    }    
}

static void sort_table( GtkWidget *widget,
                        gpointer   data )
{
    gtk_clist_set_sort_column (GTK_CLIST(linkdef_list),data);
    gtk_clist_sort (GTK_CLIST(linkdef_list));
    g_print("Inside sort_table\n");
}


static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data )
{
    /* If you return FALSE in the "delete_event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs. */
/*
    dlg = gnome_message_box_new("Do you want to quit?",GNOME_MESSAGE_BOX_QUESTION,
                                 GNOME_STOCK_BUTTON_OK,
                                 GNOME_STOCK_BUTTON_APPLY,
                                 GNOME_STOCK_BUTTON_CLOSE,
                                 NULL);
    
    gtk_signal_connect(GTK_OBJECT(dlg),"clicked",
                       GTK_SIGNAL_FUNC(messagebox_clicked),NULL);
                       
    gtk_widget_show(dlg);
*/
    /* Change TRUE to FALSE and the main window will be destroyed with
     * a "delete_event". */

    return TRUE;
}

/* Another callback */
static void win_destroy( GtkWidget *widget,
                     gpointer   data )
{
    gtk_main_quit ();
}
/*
static void common_callback( GtkWidget *widget,
                             gpointer   data )
{
    g_print("Item Selected\n");
}
*/
/*
static void messagebox_clicked(GnomeDialog *dlg,gint button,gpointer data)
{
    switch(button)  
    {
        case 1:
            g_print("APPLY\n");
            return;
        case 0:
            g_print("OK\n");
            break;
        case 2:
            g_print("CLOSE\n");
            break;
    }
    gnome_dialog_close(dlg);
}
*/
/*                                         
    radio1 = gtk_radio_button_new_with_label(group,"Sumant");
    gtk_box_pack_start(GTK_BOX(vbox2),radio1,expand,fill,padding);
    group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio1));
    
    radio2 = gtk_radio_button_new_with_label(group,"Akshay");
    gtk_box_pack_start(GTK_BOX(vbox2),radio2,expand,fill,padding);
    group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio2));
*/  
void del_message_func(Message *m)
{
    // free contents of m only if they were allocated dynamically.
}


