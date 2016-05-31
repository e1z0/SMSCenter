/*
 SMSCenter send sms program using GNOKII Libraries
 Copyright 2016 (c) justinas@eofnet.lt, EofNET LAB06.
 Compile:
 gcc -o sms sms.c -I/opt/gnokii/include -L`pwd` -lgnokii -lpthread -std=c99
 Usage:
 ./sms 370xxxxxxxx "TEXT Message"
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gnokii.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdio.h>

static struct gn_statemachine *state=NULL;

void bus_init(void){
    gn_lib_phoneprofile_load_from_file("/etc/gnokii.conf",NULL,&state);
    gn_lib_phone_open(state);
}

void bus_terminate(void){
    gn_lib_phone_close(state);
    gn_lib_phoneprofile_free(&state);
}

int main(int argc, char * argv []) {
static gn_sms_message_center msg_center;
const char *nr = NULL;
char msg[850];

// loop trough parameters
    for( int i = 0; i < argc; ++i ) {
    if (i==1) nr = argv[i];
    if (i>=2) strcpy(msg, argv[i]);
}


if (argc >=2) {
printf("Pasiruosimas siusti SMS...\r\n");
} else {
printf("Naudojimas: %s nr tekstas\r\n",argv[0]);
return 1;
}

if (strlen(argv[1]) != 11) {
printf("Blogai nurodytas telefono nr: %s\r\n",nr);
return 1;
}

    for( int i = 0; i < argc; ++i ) {
    if (i==1) nr = argv[i];
    if (i>=2) strcpy(msg, argv[i]);
  }

printf("Siunciam %s i nr: %s ilgis %d\r\n",msg,nr,strlen(msg));
    gn_data *data;
    gn_sms sms;
    if(!state){
	bus_init();
        data=&state->sm_data;
	data->sms=&sms;
	gn_sms_default_submit(data->sms);
        data->message_center=&msg_center;
	data->message_center->id=1;
	gn_sm_functions(GN_OP_GetSMSCenter, data,   state);
    }
    data=&state->sm_data;
    data->sms=&sms;
    gn_sms_default_submit(data->sms);
    snprintf(sms.smsc.number, sizeof(sms.smsc.number), "%s",msg_center.smsc.number);
    sms.smsc.type=msg_center.smsc.type;
    snprintf(sms.remote.number,sizeof(sms.remote.number), "%s", nr);
    sms.remote.type=GN_GSM_NUMBER_Unknown;
    sms.user_data[0].type=GN_SMS_DATA_Text;
//    sprintf(sms.user_data[0].u.text, "%.911s", msg);
    snprintf(sms.user_data[0].u.text,sizeof(sms.user_data[0].u.text),"%s",msg);
    sms.user_data[0].length=strlen(sms.user_data[0].u.text);
    sms.user_data[1].type=GN_SMS_DATA_None;
    gn_sms_send(data, state);
    bus_terminate();
    state=NULL;
return 0;
}
