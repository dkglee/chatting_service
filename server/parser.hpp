#ifndef PARSER_HPP
# define PARSER_HPP

# include <utility>
# include <string>

class Parser {
public:
	typedef std::pair<std::string, std::string> ParseData;
	Parser();
	~Parser();
	static ParseData parseForLogin(char* buf, int bytes_read);
	static ParseData parseForMessage(char* buf, int bytes_read);
};

#endif


// 파서가 파싱한 내용을 가지고 기능을 결정한 다음에 팩토리 패턴을 이용해서 기능 객체를 생성하고, 그 객체를 반환하는 형식
