#ifndef STRINGPOOL_H
#define STRINGPOOL_H

#include <algorithm>
#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>
#include <string_view>
#include <fstream>

struct alignas(sizeof(uint32_t) * 2) StringRef {
	uint32_t offset = 0;
	uint32_t length = 0;

	bool operator==(const StringRef& other) const {
		return (offset == other.offset && length == other.length);
	}
};

class StringPool
{
private:
	std::vector<char> buffer; // the pool
	std::unordered_map<std::string, StringRef> lookup; // optional map for deduplication
public:
	StringPool(); // start with a null terminator to simplify empty string handling
	void Clear();
	void Reserve(size_t capacity);

	StringRef Add(const std::string& str);

	[[nodiscard]] std::string_view Get(StringRef ref) const;

	void SaveToStream(std::ofstream& out) const;

	void LoadFromStream(std::ifstream& in);

	bool LessThan(StringRef a, StringRef b) {
		return this->Get(a) < this->Get(b);
	}
};

#endif