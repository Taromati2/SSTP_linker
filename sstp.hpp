#include <map>
#include "base.hpp"
struct SSTP_link_arg_parts_t{
	string_t _name;
	string_t _var;
};
struct SSTP_link_args_t{
	std::map<string_t,string_t>_m;
	SSTP_link_args_t(std::initializer_list<SSTP_link_arg_parts_t>a){
		for (auto&x:a) {
			_m[x._name]=x._var;
		}
	}
	SSTP_link_args_t(string_t a){
		if(a.find(":") > a.find("\r\n"))
			a.erase(0,a.find("\r\n"));
		while(a!=""){
			string_t t=a.substr(0,a.find("\r\n"));
			if (t.find(": ")!=t.npos){
				_m[t.substr(0,t.find(": "))]=t.substr(t.find(": ")+2);
				a.erase(0,a.find("\r\n")+2);
				while (!a.substr(0,a.find("\r\n")).find(": ")){
					_m[t.substr(0,t.find(": "))]+="\r\n"+a.substr(0,a.find("\r\n"));
					a.erase(0,a.find("\r\n")+2);
				}
			}else{
				a.erase(0,a.find("\r\n")+2);
			}
		}
	}
	template<class T>
	SSTP_link_args_t(T&&a):SSTP_link_args_t(string_t(a)){}
	operator string_t(){
		string_t aret;
		for (auto&x:_m) {
			aret += x.first + ": " + x.second + "\r\n";
		}
		return aret;
	}
	auto operator[](string_t a){return _m[a];}
};

template<class T>
auto operator+(T&&a,SSTP_link_args_t&b) {return a+string_t(b);}

struct SSTP_ret_t{
	string_t _m;
	SSTP_ret_t(string_t a):_m(a){}
	operator string_t(){return _m;}
	explicit operator SSTP_link_args_t(){return _m;}
	auto to_str(){return _m;}
	auto to_map(){return operator SSTP_link_args_t();}
	auto operator[](string_t a){return SSTP_link_args_t(_m)[a];}
};

template<class T>
auto&&operator<<(T&&a,SSTP_ret_t b) {return a<<b.to_str();}
template<class T>
auto operator+(T&&a,SSTP_ret_t b) {return a+b.to_str();}

struct SSTP_link_t:Socket_link_t{
	SSTP_link_args_t _header;

	SSTP_link_t(
				SSTP_link_args_t header={{"Charset","UTF-8"},{"Sender","void"}},
				string_t addr="127.0.0.1",unsigned int port=9821
				):
		Socket_link_t(addr,port),_header(header){}

	string_t get_SSTP_head(string_t SSTP_type){
		return SSTP_type+"\r\n"+_header;
	}
	string_t base_SSTP_send(string_t head,SSTP_link_args_t args){
		base_send(get_SSTP_head(head)+args+"\r\n");
		auto aret = base_get_ret();
		relink();
		return aret;
	}
	SSTP_ret_t NOTYFY(SSTP_link_args_t args){
		return base_SSTP_send("NOTIFY SSTP/1.1",args);
	}
	SSTP_ret_t SEND(SSTP_link_args_t args){
		return base_SSTP_send("SEND SSTP/1.4",args);
	}
	SSTP_ret_t EXECUTE(SSTP_link_args_t args){
		return base_SSTP_send("EXECUTE SSTP/1.3",args);
	}
	SSTP_ret_t COMMUNICATE(SSTP_link_args_t args){
		return base_SSTP_send("COMMUNICATE SSTP/1.1",args);
	}
	SSTP_ret_t GIVE(SSTP_link_args_t args){
		return base_SSTP_send("GIVE SSTP/1.1",args);
	}
	SSTP_ret_t INSTALL(SSTP_link_args_t args){
		return base_SSTP_send("INSTALL SSTP/1.0",args);
	}
};
