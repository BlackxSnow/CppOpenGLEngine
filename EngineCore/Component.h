#pragma once
#include <cstddef>
#include <string>

class SceneObject;

#define TO_STRING( x ) #x

// Macros for generating component declaration and definition code for type and inheritance checking at runtime (RTTI)
#define COMPONENT_DECLARATION( classname )                                                  \
public:                                                                                     \
    static const std::size_t Type;                                                          \
    virtual bool IsClassType( const std::size_t classType ) const override;   


#define COMPONENT_DEFINITION( parentclass, childclass )                                         \
const std::size_t childclass::Type = std::hash< std::string >()( TO_STRING( childclass ) ); \
bool childclass::IsClassType( const std::size_t classType ) const {                         \
        if ( classType == childclass::Type )                                                \
            return true;                                                                    \
        return parentclass::IsClassType( classType );                                       \
}                                                   

/// <summary>
/// Abstract base class for all SceneObject components.
/// </summary>
class Component
{
public:
	SceneObject* GetSceneObject();
	Component(SceneObject* attached);

    static const std::size_t Type;
    virtual bool IsClassType(const std::size_t classType) const;
	
    virtual void Disable();
    virtual void Enable();

    bool IsEnabled();

protected:
    bool IsActive;
	SceneObject* AttachedObject;
};