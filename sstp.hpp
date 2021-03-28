#include "base.hpp"

struct SSTP_link_t:Socket_link_t{
	string_t _charset="UTF-8";
	string_t _sender_name="void";
	SSTP_link_t(string_t addr="127.0.0.1",unsigned int port=9821):Socket_link_t(addr,port){}

	string_t get_SSTP_head(string_t SSTP_type){
		return
		SSTP_type+"\r\n"+
		"Charset: "+_charset+"\r\n"+
		"Sender: "+_sender_name+"\r\n";
	}
	string_t base_SSTP_send(string_t head,string_t args){
		base_send(get_SSTP_head(head)+args+"\r\n");
		return base_get_ret();
	}
	string_t NOTYFY(string_t args){
		return base_SSTP_send("NOTIFY SSTP/1.1",args);
	}
	string_t SEND(string_t args){
		return base_SSTP_send("SEND SSTP/1.4",args);
	}
	string_t EXECUTE(string_t args){
		return base_SSTP_send("EXECUTE SSTP/1.3",args);
	}
	string_t COMMUNICATE(string_t args){
		return base_SSTP_send("COMMUNICATE SSTP/1.1",args);
	}
	string_t GIVE(string_t args){
		return base_SSTP_send("GIVE SSTP/1.1",args);
	}
	string_t INSTALL(string_t args){
		return base_SSTP_send("INSTALL SSTP/1.0",args);
	}
};
