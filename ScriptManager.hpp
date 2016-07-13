#ifndef SCRIPTMANAGER_HPP
#define SCRIPTMANAGER_HPP

#include <unordered_map>
#include <vector>

#include <selene.h>

class ScriptManager
{
    public:
        ScriptManager() : mScripts() {}

        sel::State& operator[](std::string const& script)
        {
            if (mScripts.find(script) == mScripts.end())
            {
                mScripts.emplace(script, true);
                mScripts[script].Load(script);
                for (auto& lib : mLibraries)
                {
                    if (lib)
                    {
                        lib(mScripts[script]);
                    }
                }
            }
            return mScripts.at(script);
        }

        bool isLoaded(std::string const& script) const
        {
            return (mScripts.find(script) == mScripts.end());
        }

        void release(std::string const& script)
        {
            auto itr = mScripts.find(script);
            if (itr != mScripts.end())
            {
                mScripts.erase(itr);
            }
        }

        void addLibrary(std::function<void(sel::State& state)> library = [](sel::State& state){})
        {
            mLibraries.push_back(library);
        }

    private:
        std::unordered_map<std::string,sel::State> mScripts;
        std::vector<std::function<void(sel::State& state)>> mLibraries;
};

#endif // SCRIPTMANAGER_HPP
