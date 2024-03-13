#include "config.hpp"

#include <ShlObj_core.h>
#include <Windows.h>
#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>

constexpr std::string_view CONFIG_FOLDER = "KiwiCheats";
constexpr std::string_view JSON_POST_FIX = ".json";

config cfg;

namespace settings {
    item::item(const uint32_t name, const uint32_t type) : name(name), type(type) {}

    std::vector<item>& get_items() {
        static std::vector<item> items;
        return items;
    }

    item& get_item(const uint32_t index) {
        return get_items()[index];
    }

    int32_t does_item_exist(const uint32_t name) {
        for (auto i = 0u; i < get_items().size(); ++i) {
            if (get_items()[i].name == name)
                return i;
        }

        return -1;
    }

    std::filesystem::path config_folder() {
        std::string documents_path;
        documents_path.reserve(MAX_PATH);

        if (!SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_MYDOCUMENTS, nullptr, 0, documents_path.data()))) {
            return {};
        }

        auto config_folder_path = std::filesystem::path(documents_path.c_str()) / CONFIG_FOLDER;

        if (!std::filesystem::exists(config_folder_path))
            std::filesystem::create_directory(config_folder_path);

        return config_folder_path;
    }

    bool list(std::vector<std::string>& configs) {
        const auto config_path = config_folder();
        if (config_path.empty())
            return false;

        for (const auto& entry: std::filesystem::directory_iterator(config_path)) {
            if (std::filesystem::is_directory(entry))
                continue;

            const auto& file_path = entry.path();

            if (file_path.extension() != JSON_POST_FIX)
                continue;

            configs.emplace_back(std::move(file_path.filename().replace_extension().string()));
        }

        return true;
    }

    bool load(const std::string& name) {
        std::unique_lock lock_guard(config_mutex);

        if (name.empty())
            return false;

        const auto config_path = config_folder();
        if (config_path.empty())
            return false;

        const auto file_name = std::string(name.c_str()) + std::string(JSON_POST_FIX);
        const auto file_path = config_path / file_name;

        if (!std::filesystem::exists(file_path) || file_size(file_path) <= 0)
            return false;

        std::ifstream stream(file_path);
        if (!stream.is_open())
            return false;

        nlohmann::json json;
        stream >> json;

        for (auto& item: json) {
            auto index = does_item_exist(item["name"].get<uint32_t>());
            if (index < 0)
                continue;

            auto& current = get_item(index);

            switch (item["type"].get<uint32_t>()) {
                case carbon::cx_hash("bool"): {
                    current.set(item["value"].get<bool>());
                    break;
                }
                case carbon::cx_hash("int32_t"): {
                    current.set(item["value"].get<int32_t>());
                    break;
                }
                case carbon::cx_hash("float"): {
                    current.set(item["value"].get<float>());
                    break;
                }
                case carbon::cx_hash("std::vector<bool>"): {
                    const auto& vec = nlohmann::json::parse(item["value"].get<std::string>());
                    auto& item_vec = current.get<std::vector<bool>>();
                    for (size_t i = 0; i < vec.size(); ++i) {
                        if (i < item_vec.size())
                            item_vec[i] = vec[i].get<bool>();
                    }
                    break;
                }
                case carbon::cx_hash("std::vector<int32_t>"): {
                    const auto& vec = nlohmann::json::parse(item["value"].get<std::string>());
                    auto& item_vec = current.get<std::vector<int32_t>>();
                    for (size_t i = 0; i < vec.size(); ++i) {
                        if (i < item_vec.size())
                            item_vec[i] = vec[i].get<int32_t>();
                    }
                    break;
                }
                case carbon::cx_hash("std::vector<float>"): {
                    const auto& vec = nlohmann::json::parse(item["value"].get<std::string>());
                    auto& item_vec = current.get<std::vector<float>>();
                    for (size_t i = 0; i < vec.size(); ++i) {
                        if (i < item_vec.size())
                            item_vec[i] = vec[i].get<float>();
                    }
                    break;
                }
                case carbon::cx_hash("glm::vec4"): {
                    const auto& vec = nlohmann::json::parse(item["value"].get<std::string>());
                    current.set<glm::vec4>(glm::vec4(vec[0].get<float>(), vec[1].get<float>(), vec[2].get<float>(), vec[3].get<float>()));
                    break;
                }
                    //case util::cx_hash("filter_ids_type"): {
                    //    const auto& json_list = nlohmann::json::parse(item["value"].get<std::string>());
                    //    auto& list = current.get<std::list<std::pair<std::wstring, std::string>>>();
                    //    list.clear();

                    //    for (const auto& filter_pair : json_list) {
                    //        list.emplace_back(std::move(filter_pair.get<std::pair<std::wstring, std::string>>()));
                    //    }
                    //    break;
                    //}
            }
        }

        // load_complex_structures();

        return true;
    }

    bool save(const std::string& name) {
        std::unique_lock lock_guard(config_mutex);

        if (name.empty())
            return false;

        const auto config_path = config_folder();
        if (config_path.empty())
            return false;

        const auto file_name = std::string(name.c_str()) + std::string(JSON_POST_FIX);
        const auto file_path = config_path / file_name;
        std::ofstream stream(file_path);
        if (!stream.is_open())
            return false;

        nlohmann::json json;

        // save_complex_structures();

        for (auto& item: get_items()) {
            nlohmann::json current;
            current["name"] = item.name;
            current["type"] = item.type;

            switch (item.type) {
                case carbon::cx_hash("bool"): {
                    current["value"] = item.get<bool>();
                    break;
                }
                case carbon::cx_hash("int32_t"): {
                    current["value"] = item.get<int32_t>();
                    break;
                }
                case carbon::cx_hash("float"): {
                    current["value"] = item.get<float>();
                    break;
                }
                case carbon::cx_hash("std::vector<bool>"): {
                    const auto& vec = item.get<std::vector<bool>>();
                    nlohmann::json sub;
                    for (auto v: vec)
                        sub.push_back(v);
                    current["value"] = sub.dump();
                    break;
                }
                case carbon::cx_hash("std::vector<int32_t>"): {
                    const auto& vec = item.get<std::vector<int32_t>>();
                    nlohmann::json sub;
                    for (auto v: vec)
                        sub.push_back(v);
                    current["value"] = sub.dump();
                    break;
                }
                case carbon::cx_hash("std::vector<float>"): {
                    const auto& vec = item.get<std::vector<float>>();
                    nlohmann::json sub;
                    for (auto v: vec)
                        sub.push_back(v);
                    current["value"] = sub.dump();
                    break;
                }
                case carbon::cx_hash("glm::vec4"): {
                    auto vec = item.get<glm::vec4>();
                    nlohmann::json sub;
                    sub.push_back(vec.x);
                    sub.push_back(vec.y);
                    sub.push_back(vec.z);
                    sub.push_back(vec.w);
                    current["value"] = sub.dump();
                    break;
                }
                    // case util::cx_hash("filter_ids_type"): {
                    //     const auto& list = item.get<std::list<std::pair<std::wstring, std::string>>>();
                    //     nlohmann::json json_list;

                    //     for (const auto& pair : list) {
                    //         json_list.emplace_back(std::move(nlohmann::json(pair)));
                    //     }

                    //     current["value"] = json_list.dump();
                    //     break;
                    // }
            }

            json.push_back(current);
        }

        stream << json.dump(4) << std::endl;

        return true;
    }

    bool erase(const std::string& name) {
        if (name.empty())
            return false;

        const auto config_path = config_folder();
        if (config_path.empty())
            return false;

        const auto file_name = std::string(name.c_str()) + std::string(JSON_POST_FIX);
        const auto file_path = config_path / file_name;
        if (!std::filesystem::exists(file_path) || std::filesystem::is_directory(file_path))
            return false;

        return std::filesystem::remove(file_path);
    }
} // namespace settings