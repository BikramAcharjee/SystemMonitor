#pragma once
#include <string>
#include <Windows.h>
#include <wbemidl.h>

class WmiClient {
public:
	WmiClient();
	~WmiClient();

	WmiClient(const WmiClient&) = delete;
	WmiClient& operator=(const WmiClient&) = delete;

	bool initialize();
	std::string getProperty(
		const std::wstring& query,
		const std::wstring& property
	);

private:
	IWbemServices* services_;
	bool initialized_;

	std::string wideToUtf8(const std::wstring& value);
};