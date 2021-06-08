//copy from https://github.com/ponapalt/csaori
//
#include <vector>
#include <Windows.h>

namespace CODEPAGE_n{
	#undef CP_UTF8
	typedef enum {
		CP_SJIS=932,
		CP_EUCJP=20932,
		CP_ISO2022JP=50220,
		CP_UTF8=65001
	} CODEPAGE;

	std::string UnicodeToMultiByte(const wchar_t *Source, unsigned int CodePage, DWORD Flags)
	{
		if ( Source && *Source ) {
			if (int Len = ::WideCharToMultiByte(CodePage, Flags, Source, wcslen(Source), NULL, 0, NULL, NULL)) {
				std::vector<char> Dest(Len);
				if (Len = ::WideCharToMultiByte(CodePage, Flags, Source, wcslen(Source), &Dest[0], static_cast<int>(Dest.size()), NULL, NULL)) {
					return std::string(Dest.begin(), Dest.begin() + Len);
				}
			}
		}
		return "";
	}

	std::wstring MultiByteToUnicode(const char* Source, unsigned int CodePage, DWORD Flags)
	{
		if ( Source && *Source ) {
			if (int Len = ::MultiByteToWideChar(CodePage, Flags, Source, strlen(Source), NULL, 0)) {
				std::vector<wchar_t> Dest(Len);
				if (Len = ::MultiByteToWideChar(CodePage, Flags, Source, strlen(Source), &Dest[0], static_cast<int>(Dest.size()))) {
					return std::wstring(Dest.begin(), Dest.begin() + Len);
				}
			}
		}
		return L"";
	}

	std::string UnicodeToMultiByte(std::wstring Source, unsigned int CodePage, DWORD Flags)
	{
		if ( Source.size() ) {
			if (int Len = ::WideCharToMultiByte(CodePage, Flags, Source.c_str(), Source.size(), NULL, 0, NULL, NULL)) {
				std::vector<char> Dest(Len);
				if (Len = ::WideCharToMultiByte(CodePage, Flags, Source.c_str(), Source.size(), &Dest[0], static_cast<int>(Dest.size()), NULL, NULL)) {
					return std::string(Dest.begin(), Dest.begin() + Len);
				}
			}
		}
		return "";
	}

	std::wstring MultiByteToUnicode(std::string Source, unsigned int CodePage, DWORD Flags)
	{
		if ( Source.size() ) {
			if (int Len = ::MultiByteToWideChar(CodePage, Flags, Source.c_str(), Source.size(), NULL, 0)) {
				std::vector<wchar_t> Dest(Len);
				if (Len = ::MultiByteToWideChar(CodePage, Flags, Source.c_str(), Source.size(), &Dest[0], static_cast<int>(Dest.size()))) {
					return std::wstring(Dest.begin(), Dest.begin() + Len);
				}
			}
		}
		return L"";
	}

	std::wstring CodePagetoString(unsigned int cset){
		switch(cset){
			case CP_SJIS:
				return L"Shift_JIS";
			case CP_ISO2022JP:
				return L"ISO-2022-JP";
			case CP_EUCJP:
				return L"EUC-JP";
			case CP_UTF8:
				return L"UTF-8";
		}
		return L"unknown charset";
	}

	UINT StringtoCodePage(const char *str)
	{
		if ( str && *str ) {
			if ( _strnicmp(str,"shift_jis",9) == 0 ) {
				return CP_SJIS;
			}
			if ( _strnicmp(str,"x-sjis",6) == 0 ) {
				return CP_SJIS;
			}
			if ( _strnicmp(str,"iso-2022-jp",11) == 0 ) {
				return CP_ISO2022JP;
			}
			if ( _strnicmp(str,"euc-jp",6) == 0 ) {
				return CP_EUCJP;
			}
			if ( _strnicmp(str,"x-euc-jp",8) == 0 ) {
				return CP_EUCJP;
			}
			if ( _strnicmp(str,"utf-8",5) == 0 ) {
				return CP_UTF8;
			}
		}
		return CP_SJIS;
	}

	UINT StringtoCodePage(const wchar_t *str)
	{
		if ( str && *str ) {
			if ( _wcsnicmp(str,L"shift_jis",9) == 0 ) {
				return CP_SJIS;
			}
			if ( _wcsnicmp(str,L"x-sjis",6) == 0 ) {
				return CP_SJIS;
			}
			if ( _wcsnicmp(str,L"iso-2022-jp",11) == 0 ) {
				return CP_ISO2022JP;
			}
			if ( _wcsnicmp(str,L"euc-jp",6) == 0 ) {
				return CP_EUCJP;
			}
			if ( _wcsnicmp(str,L"x-euc-jp",8) == 0 ) {
				return CP_EUCJP;
			}
			if ( _wcsnicmp(str,L"utf-8",5) == 0 ) {
				return CP_UTF8;
			}
		}
		return CP_SJIS;
	}
}
