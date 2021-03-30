#include <iostream>
#include "sstp.hpp"

int main() {
	SSTP_link_n::SSTP_link_t link;
	std::wcout << link.NOTYFY({{L"Event", L"OnCommunicate"},
							   {L"Reference0", L"user"},
							   {L"Reference1", L"你好"}});
	std::wcout << link.NOTYFY({{L"Event", L"OnCommunicate"},
							   {L"Reference0", L"user"},
							   {L"Reference1", L"近来可好"}})[L"Script"];
}

