#include "socket.hpp"
#include "codepage.hpp"
#include <map>

namespace SSTP_link_n{
	using namespace CODEPAGE_n;
	using namespace Socket_link_n;
	struct SSTP_link_arg_parts_t{
		std::wstring _name;
		std::wstring _var;
	};
	struct SSTP_link_args_t{
		std::map<std::wstring,std::wstring>_m;
		SSTP_link_args_t(std::initializer_list<SSTP_link_arg_parts_t>a){
			for (auto&x:a) {
				_m[x._name]=x._var;
			}
		}
		explicit SSTP_link_args_t(std::wstring a){
			if(a.find(L":") > a.find(L"\r\n"))
				a.erase(0,a.find(L"\r\n"));
			while(a!=L""){
				std::wstring t=a.substr(0,a.find(L"\r\n"));
				if (t.find(L": ")!=t.npos){
					_m[t.substr(0,t.find(L": "))]=t.substr(t.find(L": ")+2);
					a.erase(0,a.find(L"\r\n")+2);
					while (!a.substr(0,a.find(L"\r\n")).find(L": ")){
						_m[t.substr(0,t.find(L": "))]+=L"\r\n"+a.substr(0,a.find(L"\r\n"));
						a.erase(0,a.find(L"\r\n")+2);
					}
				}else{
					a.erase(0,a.find(L"\r\n")+2);
				}
			}
		}
		template<class T>
		explicit SSTP_link_args_t(T&&a):SSTP_link_args_t(std::wstring(a)){}
		explicit operator std::wstring(){
			std::wstring aret;
			for (auto&x:_m) {
				aret += x.first + L": " + x.second + L"\r\n";
			}
			return aret;
		}
		auto var(){return _m[L"Script"];}
		bool has(std::wstring a){return _m.count(L"X-SSTP-Return-"+a);}
		auto operator[](std::wstring a){return _m[L"X-SSTP-Return-"+a];}
	};

	template<class T>
	auto operator+(T&&a,SSTP_link_args_t&b) {return std::wstring(a)+std::wstring(b);}

	struct SSTP_ret_t{
		std::wstring _m;
		SSTP_ret_t(std::wstring a):_m(a){}
		operator std::wstring(){return _m;}
		explicit operator SSTP_link_args_t(){return SSTP_link_args_t(_m);}
		auto to_str(){return _m;}
		auto to_map(){return operator SSTP_link_args_t();}
		auto operator[](std::wstring a){return to_map()[a];}
		auto get_head(){return _m.substr(_m.find(L"SSTP/"),_m.find(L"\r\n"));}
		auto get_code(){return std::stoi(_m.substr(_m.find(L"SSTP/")+9,_m.find(L"SSTP/")+9+3));}
		auto var(){return to_map().var();}
		bool has(std::wstring a){return to_map().has(a);}
	};

	template<class T>
	auto&&operator<<(T&&a,SSTP_ret_t&b) {return a<<b.to_str();}
	template<class T>
	auto operator+(T&&a,SSTP_ret_t&b) {return a+b.to_str();}

	struct SSTP_link_t:Socket_link_t{
		SSTP_link_args_t _header;

		SSTP_link_t(
					SSTP_link_args_t header={{L"Charset",L"UTF-8"},{L"Sender",L"void"}},
					std::string addr="127.0.0.1",unsigned int port=9821
					):
			Socket_link_t(addr,port),_header(header){}

		std::wstring get_SSTP_head(std::wstring SSTP_type){
			return SSTP_type+L"\r\n"+_header;
		}
		std::wstring base_SSTP_send(std::wstring head,SSTP_link_args_t args){
			relink();//SSTP server can't process "keep-alive" style connection like HTTP: http://ssp.shillest.net/bts/view.php?id=170#c384
			{
				auto send=get_SSTP_head(head)+args+L"\r\n";
				auto charset_begin=send.find(L"\r\nCharset: ")+11;
				auto charset=send.substr(charset_begin,send.find(L"\r\n",charset_begin));
				base_send(UnicodeToMultiByte(send,StringtoCodePage(charset.c_str()),0));
			}
			{
				auto temp = base_get_ret();
				auto charset_begin=temp.find("\r\nCharset: ")+11;
				auto charset=temp.substr(charset_begin,temp.find("\r\n",charset_begin));
				return MultiByteToUnicode(temp,StringtoCodePage(charset.c_str()),0);
			}
		}
		SSTP_ret_t NOTYFY(SSTP_link_args_t args){
			return base_SSTP_send(L"NOTIFY SSTP/1.1",args);
		}
		SSTP_ret_t SEND(SSTP_link_args_t args){
			return base_SSTP_send(L"SEND SSTP/1.4",args);
		}
		SSTP_ret_t EXECUTE(SSTP_link_args_t args){
			return base_SSTP_send(L"EXECUTE SSTP/1.3",args);
		}
		SSTP_ret_t COMMUNICATE(SSTP_link_args_t args){
			return base_SSTP_send(L"COMMUNICATE SSTP/1.1",args);
		}
		SSTP_ret_t GIVE(SSTP_link_args_t args){
			return base_SSTP_send(L"GIVE SSTP/1.1",args);
		}
		SSTP_ret_t INSTALL(SSTP_link_args_t args){
			return base_SSTP_send(L"INSTALL SSTP/1.0",args);
		}
		/*
		define this in aya:
		SHIORI_EV.On_Has_Event : void {
			SHIORI_FW.MakeXSSTPreturn('Result',ISFUNC(reference0)||ISFUNC('On_'+reference0)||ISFUNC('SHIORI_EV.'+reference0)||ISFUNC('SHIORI_EV.On_'+reference0))
		}
		*/
		bool Has_Event(std::wstring event_name){
			return NOTYFY({{ L"Event", L"Has_Event" },{ L"Reference0", event_name }})[L"Result"]==L"1";
		}
	};
}
