#ifndef __ModuleFrame__
#define __ModuleFrame__

#include "axLib.h"

#include <map>

template<class T>
class ModuleFrame: public axPanel
{
public:
    ModuleFrame(axWindow* parent,
                const axRect& rect):
    axPanel(parent, rect)
    {
        
    }
    
protected:
    axDropMenu* _moduleMenu;
    std::map<std::string, int> _nameIndex;
    
    void AddModule(T* module, const std::string& name)//, const int& index)
    {
        int index = (int)_moduleVector.size();
        
//        std::cout << "Index : " << index << std::endl;
//        
//        module->Hide();
        if(index != 0)
        {
            module->Hide();
        }
//        module->Show();
        
//        std::cout << "Module show :: " << module->IsShown() << std::endl;
        
//        Update();
        
        _moduleVector.push_back(module);
        _nameIndex.insert(std::pair<std::string, int>(name, index));
    }
    
    int GetIndex(const std::string& name)
    {
        std::map<std::string, int>::iterator it = _nameIndex.find(name);
        if(it != _nameIndex.end())
        {
            return it->second;
        }
        
        return -1;
    }
    
    void SetCurrentModule(const int& index)
    {
        for(auto n : _moduleVector)
        {
            n->Hide();
        }
        
        _moduleVector[index]->Show();
    }
    
    std::vector<T*> _moduleVector;
    
protected:
    axEVENT_ACCESSOR(axDropMenuMsg, OnModuleChange);
private:
    
    void OnModuleChange(const axDropMenuMsg& msg)
    {
        int moduleIndex = GetIndex(msg.GetMsg());
        if(moduleIndex != -1)
        {
            SetCurrentModule(moduleIndex);
        }
    }
    
    // Events.
    virtual void OnPaint()
    {
        axGC* gc = GetGC();
        axRect rect(axPoint(0, 0), GetRect().size);
        
//        gc->SetColor(axColor(0.15, 0.15, 0.15), 1.0);
//        gc->DrawRectangle(rect);

        gc->SetColor(axColor(0.2, 0.2, 0.2), 1.0);
        gc->DrawRectangle(axRect(0, 0, rect.size.x, 17));

        gc->SetColor(axColor(0.4, 0.4, 0.4), 1.0);
        gc->DrawRectangleContour(rect);
        
    }
};

#endif // __ModuleFrame__
