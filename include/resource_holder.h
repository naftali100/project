#pragma once
#ifndef resources_holder_h
#define resources_holder_h

#include <cassert>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

template <typename Resource, typename Identifier>
class ResourceHolder {
public:
    static void load(Identifier id, const std::string& filename) {
        Instance().loadResource(id, filename);
    }

    template <typename Parameter>
    static void load(Identifier id, const std::string& filename, const Parameter& secondParam) {
        Instance().loadResource(id, filename, secondParam);
    }

    static void set(Identifier id, const Resource& r) {
        std::unique_ptr<Resource> ptr = std::make_unique<Resource>(std::move(r));
        Instance().insertResource(id, std::move(ptr));
    }
    static void set(Identifier id, std::unique_ptr<Resource> r) {
        Instance().insertResource(id, std::move(r));
    }

    static Resource& get(Identifier id) {
        return Instance().getResource(id);
    }

    static void unload(Identifier i) {
        Instance().mResourceMap[i] = nullptr;
    }

private:
    static ResourceHolder& Instance() {
        static ResourceHolder instance;
        return instance;
    }

    Resource& getResource(Identifier id);
    const Resource& getResource(Identifier id) const;

    void loadResource(Identifier id, const std::string& filename);

    template <typename Parameter>
    void loadResource(Identifier id, const std::string& filename,
        const Parameter& secondParam);

private:
    ResourceHolder() = default;
    ResourceHolder(const ResourceHolder&) = default;
    ResourceHolder operator=(ResourceHolder other) = delete;
    void insertResource(Identifier id, std::unique_ptr<Resource> resource);

    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

#include "resource_holder.tcc"

#endif