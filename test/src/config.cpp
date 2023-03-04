#include "config.hpp"

#include <fstream>
#include <iomanip>

#include <Shlobj.h>
#include <Windows.h>

#define CONFIG_FOLDER L"KiwiCheats"

cfg_t cfg;

namespace config {
	item::item(const uint32_t name, const uint32_t type, const std::any var) : name(name), type(type), var(var) {}

	std::vector<item>& get_items() {
		static std::vector<item> items;
		return items;
	}

	item& get_item(const uint32_t index) {
		return get_items()[index];
	}

	int does_item_exist(const uint32_t name) {
		for (auto i = 0u; i < get_items().size(); ++i) {
			if (get_items()[i].name == name)
				return i;
		}

		return -1;
	}

	std::wstring config_folder() {
		WCHAR path[MAX_PATH];
		if (!SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_COMMON_APPDATA, nullptr, 0, path))) {
			return {};
		}

		auto result = std::wstring(path) + (L"\\" CONFIG_FOLDER L"\\");

		CreateDirectoryW(result.c_str(), nullptr);

		return result;
	}

	bool list(std::vector<std::wstring>& configs) {
		auto folder = config_folder() + L"*";

		WIN32_FIND_DATAW find_data;
		HANDLE handle = FindFirstFileW(folder.c_str(), &find_data);

		if (handle == INVALID_HANDLE_VALUE)
			return false;

		while (FindNextFileW(handle, &find_data) != 0) {
			const std::wstring config = find_data.cFileName;
			const std::wstring ending = L".json";

			if (ending.size() > config.size())
				continue;

			if (!std::equal(ending.rbegin(), ending.rend(), config.rbegin()))
				continue;

			if (config == ending)
				continue;

			configs.emplace_back(config.substr(0, config.size() - 5));
		}

		FindClose(handle);

		return true;
	}

	bool load(const std::wstring& name) {
		std::ifstream stream(config_folder() + name + L".json");

		if (!stream.is_open())
			return false;

		nlohmann::json json;
		stream >> json;

		for (auto& item : json) {
			auto index = does_item_exist(item["name"].get<uint32_t>());
			if (index < 0)
				continue;

			auto current = get_item(index);

			switch (item["type"].get<uint32_t>()) {
				case util::cx_hash("bool"): {
						current.set(item["value"].get<bool>());
						break;
					}
				case util::cx_hash("int"): {
						current.set(item["value"].get<int>());
						break;
					}
				case util::cx_hash("float"): {
						current.set(item["value"].get<float>());
						break;
					}
				case util::cx_hash("std::vector<bool>"): {
						auto vec = nlohmann::json::parse(item["value"].get<std::string>());
						auto& item_vec = current.get<std::vector<bool>>();
						for (size_t i = 0; i < vec.size(); ++i) {
							if (i < item_vec.size())
								item_vec[i] = vec[i].get<bool>();
						}
						break;
					}
				case util::cx_hash("std::vector<int>"): {
						auto vec = nlohmann::json::parse(item["value"].get<std::string>());
						auto& item_vec = current.get<std::vector<int>>();
						for (size_t i = 0; i < vec.size(); ++i) {
							if (i < item_vec.size())
								item_vec[i] = vec[i].get<int>();
						}
						break;
					}
				case util::cx_hash("std::vector<float>"): {
						auto vec = nlohmann::json::parse(item["value"].get<std::string>());
						auto& item_vec = current.get<std::vector<float>>();
						for (size_t i = 0; i < vec.size(); ++i) {
							if (i < item_vec.size())
								item_vec[i] = vec[i].get<float>();
						}
						break;
					}
				case util::cx_hash("ImVec4"): {
						auto vec = nlohmann::json::parse(item["value"].get<std::string>());
						current.set<ImVec4>(ImVec4(vec[0].get<float>(), vec[1].get<float>(), vec[2].get<float>(), vec[3].get<float>()));
						break;
					}
			}
		}

		return true;
	}

	bool save(const std::wstring& name) {
		std::ofstream stream(config_folder() + name + L".json");

		if (!stream.is_open())
			return false;

		nlohmann::json json;

		for (auto& item : get_items()) {
			nlohmann::json current;
			current["name"] = item.name;
			current["type"] = item.type;

			switch (item.type) {
				case util::cx_hash("bool"): {
						current["value"] = item.get<bool>();
						break;
					}
				case util::cx_hash("int"): {
						current["value"] = item.get<int>();
						break;
					}
				case util::cx_hash("float"): {
						current["value"] = item.get<float>();
						break;
					}
				case util::cx_hash("std::vector<bool>"): {
						auto vec = item.get<std::vector<bool>>();
						nlohmann::json sub;
						for (auto v : vec)
							sub.push_back(v);
						current["value"] = sub.dump();
						break;
					}
				case util::cx_hash("std::vector<int>"): {
						auto vec = item.get<std::vector<int>>();
						nlohmann::json sub;
						for (auto v : vec)
							sub.push_back(v);
						current["value"] = sub.dump();
						break;
					}
				case util::cx_hash("std::vector<float>"): {
						auto vec = item.get<std::vector<float>>();
						nlohmann::json sub;
						for (auto v : vec)
							sub.push_back(v);
						current["value"] = sub.dump();
						break;
					}
				case util::cx_hash("ImVec4"): {
						auto vec = item.get<ImVec4>();
						nlohmann::json sub;
						sub.push_back(vec.x);
						sub.push_back(vec.y);
						sub.push_back(vec.z);
						sub.push_back(vec.w);
						current["value"] = sub.dump();
						break;
					}
			}

			json.push_back(current);
		}

		stream << json.dump(4) << std::endl;

		return true;
	}

	bool erase(const std::wstring& name) {
		return DeleteFileW((config_folder() + name + L".json").data());
	}
}// namespace config