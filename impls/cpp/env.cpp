#include "env.hpp"
#include "error.hpp"

EnvData::EnvData(Env outer, const vector<MalType>& binds, const vector<MalType>& exprs)
{
    outer_ = outer;
    for(size_t i = 0; i < binds.size(); ++i)
    {
        auto* symbol = dynamic_cast<MalSymbol*>(&*binds[i]);
        set(symbol->getSymbol(), exprs[i]);
    }
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
