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
/// Pure virtual base class for all SceneObject components.
/// </summary>
class Component
{
public:
	SceneObject* GetSceneObject();
	Component(SceneObject* attached);

    /// <summary>
    /// Value for Run Time Type Identification (RTTI).
    /// </summary>
    /// <param name="classType"></param>
    /// <returns></returns>
    static const std::size_t Type;
    /// <summary>
    /// Returns whether this class is of the same polymorphic type as 'classType'.
    /// </summary>
    virtual bool IsClassType(const std::size_t classType) const;
	
    /// <summary>
    /// Disable the component's behaviour.
    /// </summary>
    virtual void Disable();
    /// <summary>
    /// Enable the component's behaviour.
    /// </summary>
    virtual void Enable();

    bool IsEnabled();

protected:
    /// <summary>
    /// Whether or not the component is enabled.
    /// </summary>
    bool IsActive;
	SceneObject* AttachedObject;
};