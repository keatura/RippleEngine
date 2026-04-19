#include "Utils/StringPool.h"

StringPool::StringPool() { buffer.reserve(1024 * 1024); buffer.push_back('\0'); } // start with a null terminator to simplify empty string handling
void StringPool::Clear() {
	buffer.clear();
	lookup.clear();
	buffer.reserve(1024 * 1024);
	buffer.push_back('\0');
}
void StringPool::Reserve(size_t capacity) { buffer.reserve(capacity); }

StringRef StringPool::Add(const std::string& str) {
	if (str.empty()) {
		return { 0, 0 }; // reference to the empty string
	}
	auto it = lookup.find(str);
	if (it != lookup.end()) return it->second;

	StringRef ref;
	ref.offset = static_cast<uint32_t>(buffer.size());
	ref.length = static_cast<uint32_t>(str.size());

	buffer.insert(buffer.end(), str.begin(), str.end());

	lookup[str] = ref;
	return ref;
}

std::string_view StringPool::Get(StringRef ref) const {
	if (ref.offset + ref.length > buffer.size()) { throw std::out_of_range("StringRef is out of bounds"); }
	return std::string_view(&buffer[ref.offset], ref.length);
}

void StringPool::SaveToStream(std::ofstream& out) const {
	size_t size = buffer.size();
	out.write(reinterpret_cast<const char*>(&size), sizeof(size));
	if (size > 0) out.write(buffer.data(), size);
}

void StringPool::LoadFromStream(std::ifstream& in) {
	size_t size;
	in.read(reinterpret_cast<char*>(&size), sizeof(size));
	buffer.resize(size);
	if (size > 0) in.read(buffer.data(), size);
}