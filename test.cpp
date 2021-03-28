#include <iostream>
#include "sstp.hpp"


int main() {
	system("chcp 65001");
	SSTP_link_t link;
	std::cout << link.NOTYFY({{"Event", "OnCommunicate"},
							  {"Reference0", "user"},
							  {"Reference1", "你好"}});
	std::cout << link.NOTYFY({{"Event", "OnCommunicate"},
							  {"Reference0", "user"},
							  {"Reference1", "近来可好"}})["Script"];
}

