#include "env.hpp"
#include "error.hpp"

EnvData::EnvData(Env outer)
{
    outer_ = outer;
}

MalType EnvData::set(const string& key, MalType operation)
{
    return data_[key] = operation;
}

EnvData* EnvData::find(const string& key)
{
    if(data_.find(key) == data_.end())
    {
        if(outer_ == nullptr) throw InvalidSymbolException(key + " not found in environment");

        return outer_->find(key);
    }

    return this;
}

MalType EnvData::get(const string& key)
{
    auto* env = find(key);

    return env->data_[key];
}
