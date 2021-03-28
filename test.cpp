#include <iostream>
#include "sstp.hpp"


int main() {
	system("chcp 65001");
	SSTP_link_t link;
	std::cout << link.NOTYFY(
"\
Event: OnCommunicate\r\n\
Reference0: user\r\n\
Reference1: 你好\
"
							);
	//link.relink();
	std::cout << "NEXT is bug\n";
	std::cout << link.NOTYFY(
"\
Event: OnCommunicate\r\n\
Reference0: user\r\n\
Reference1: 你好\
"
							);
}
