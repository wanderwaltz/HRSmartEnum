//
//  HRSmartEnum.h
//  HRSmartEnum
//
//  Created by Egor Chiglintsev on 10.11.13.
//  Copyright (c) 2013 Sibers. All rights reserved.
//  Released under the MIT license.
//

#import <objc/runtime.h>
#import <objc/message.h>
#import "metamacros.h"


#pragma mark -
#pragma mark Macros to use in code

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The macro which is used to declare smart enums.
//
// Smart enum consists of two entities: a plain C enum and an Objective-C class representing the same enum.
//
// This macro declares both of these entities and should be used in header files with the following syntax:
//  
//      @HR_ENUM(MyEnum,
//               Value1,
//               Value2,
//               Value3);
//
//
// This example generates a C enum named MyEnum_t (suffix _t is added to differentiate between the C enum
// and corresponding class) and a class named MyEnum.
//
// Constants of the enum will be named MyEnumValue1, MyEnumValue2, MyEnumValue3 and will have default values
// starting from 0 to 2.
//
// For each of the enum value declared the class MyEnum will have the following (take Value1 for example):
//  * a readonly nonatomic NSInteger property named Value1 which always returns MyEnumValue1,
//  * a class method + (NSInteger) Value1 which always returns MyEnumValue1
//
// Additionally, MyEnum class will have the following methods:
//  * + (NSDictionary *) asDicitonary which returns a dictionary representation of the enum in the form of
//          @{ @"Value1" : @(MyEnumValue1), @"Value2" : @(MyEnumValue2), @"Value3" : @(MyEnumValue3) }
//  * + (NSArray *) allKeys which returns keys array of the dictionary representation
//  * + (NSArray *) allValues which returns values array of the dictionary representation
//  * - (NSDictionary *) asDictionary - instance method version of the +dictionary
//  * - (NSArray *) allKeys - instance method version of +allKeys
//  * - (NSArray *) allValues - instance method version of +allValues
//
//
// Please note that current limitations of the metamacro_foreach_cxt macros from extobjc library
// allow only 20 different values to be handled by HR_ENUM macro. This could be expanded by updating
// the extobjc library if needed.
//
// Also note that MyEnum class in the example above is only declared by using HR_ENUM macro and should be
// defined in one of the .m files in the application. This can be done using SYNTHESIZE_HR_ENUM macro which
// accepts the enum name as a parameter.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HR_ENUM(Name, ...)                                              \
interface Name : NSObject                                               \
- (NSArray *) allValues;                                                \
- (NSArray *) allKeys;                                                  \
+ (NSArray *) allValues;                                                \
+ (NSArray *) allKeys;                                                  \
@end                                                                    \
                                                                        \
metamacro_foreach_cxt(HR_ENUM_PROPERTY,      , Name, __VA_ARGS__)       \
metamacro_foreach_cxt(HR_ENUM_STATIC_METHOD, , Name, __VA_ARGS__)       \
                                                                        \
HR_ENUM_CONSTRUCT_PROPERTIES(Name, __VA_ARGS__)                         \
HR_ENUM_CONSTRUCT_CLASSMETHODS(Name, __VA_ARGS__)                       \
HR_ENUM_AS_DICTIONARY(Name, __VA_ARGS__)                                \
                                                                        \
typedef NS_ENUM(NSInteger, metamacro_concat(Name, _t))                  \
{                                                                       \
metamacro_foreach_cxt(HR_ENUM_ENUM_ELEMENT, , Name, __VA_ARGS__)        \
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Synthesizer of the enum class
//
// Provides runtime implementations for methods declared when using HR_ENUM macro.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SYNTHESIZE_HR_ENUM(CLASS)                      \
implementation CLASS                                   \
                                                       \
+ (void) initialize                                    \
{                                                      \
    if (self == [CLASS class])                         \
    {                                                  \
        hr_enum_construct_ ## CLASS ##_classmethods(); \
        hr_enum_construct_ ## CLASS ##_properties();   \
        hr_enum_construct_ ## CLASS ##_asDictionary(); \
    }                                                  \
}                                                      \
                                                       \
- (NSArray *) allValues                                \
{                                                      \
    return [[self class] allValues];                   \
}                                                      \
                                                       \
- (NSArray *) allKeys                                  \
{                                                      \
    return [[self class] allKeys];                     \
}                                                      \
                                                       \
+ (NSArray *) allValues                                \
{                                                      \
    return [[self asDictionary] allValues];            \
}                                                      \
                                                       \
+ (NSArray *) allKeys                                  \
{                                                      \
        return [[self asDictionary] allKeys];          \
}                                                      \
@end
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





#pragma mark -
#pragma mark Implementation details

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adds a property returning the proper enum element value.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HR_ENUM_PROPERTY(Index, CLASS, Name)                                                                    \
@interface CLASS(DynamicProperty_ ## Name)                                                                      \
@property (readonly, nonatomic) NSInteger Name;                                                                 \
@end                                                                                                            \
                                                                                                                \
static id hr_enum_ ## CLASS ##_property ## Name ##_implementation(id SELF, SEL CMD, ...)                        \
{                                                                                                               \
    return objc_msgSend([SELF class], CMD);                                                                     \
}                                                                                                               \
                                                                                                                \
__attribute__((unused))                                                                                         \
static void hr_enum_construct_ ## CLASS ##_property_ ## Name(void)                                              \
{                                                                                                               \
    Class theClass  = objc_getClass(# CLASS);                                                                   \
    class_addMethod(theClass, @selector(Name), &hr_enum_ ## CLASS ##_property ## Name ##_implementation, "i@"); \
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Performs a call to one of the property construction functions.
//
// This is a helper macro to be used in conjunction with metamacro_foreach_cxt when iterating property
// names of the enum class.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HR_ENUM_CONSTRUCT_PROPERTY_CALL(Index, CLASS, Name) \
    hr_enum_construct_ ## CLASS ##_property_ ## Name();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generates a function to be called from +initialize method of the enum class which adds property getters
// at runtime.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HR_ENUM_CONSTRUCT_PROPERTIES(CLASS, ...)                                \
__attribute__((unused))                                                         \
static void hr_enum_construct_ ## CLASS ##_properties(void)                     \
{                                                                               \
    metamacro_foreach_cxt(HR_ENUM_CONSTRUCT_PROPERTY_CALL, ,CLASS, __VA_ARGS__) \
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adds a class method returning the proper enum element value.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HR_ENUM_STATIC_METHOD(Index, CLASS, Name)                                                                   \
@interface CLASS(DynamicClassMethod_ ## Name)                                                                       \
+ (NSInteger) Name;                                                                                                 \
@end                                                                                                                \
                                                                                                                    \
static id hr_enum ## CLASS ##_classmethod ## Name ##_implementation(id SELF, SEL CMD, ...)                          \
{                                                                                                                   \
    return (__bridge id)(void *)Index;                                                                              \
}                                                                                                                   \
                                                                                                                    \
__attribute__((unused))                                                                                             \
static void hr_enum_construct_ ## CLASS ##_classmethod_ ## Name(void)                                               \
{                                                                                                                   \
    Class theClass  = objc_getClass(# CLASS);                                                                       \
    Class metaClass = object_getClass(theClass);                                                                    \
    class_addMethod(metaClass, @selector(Name), &hr_enum ## CLASS ##_classmethod ## Name ##_implementation, "i@");  \
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Performs a call to one of the classmethod construction functions.
//
// This is a helper macro to be used in conjunction with metamacro_foreach_cxt when iterating class method
// names of the enum class.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HR_ENUM_CONSTRUCT_CLASSMETHOD_CALL(Index, CLASS, Name) \
    hr_enum_construct_ ## CLASS ##_classmethod_ ## Name();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generates a function to be called from +initialize method of the enum class which adds class methods
// at runtime.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HR_ENUM_CONSTRUCT_CLASSMETHODS(CLASS, ...)                                  \
__attribute__((unused))                                                             \
static void hr_enum_construct_ ## CLASS ##_classmethods(void)                       \
{                                                                                   \
    metamacro_foreach_cxt(HR_ENUM_CONSTRUCT_CLASSMETHOD_CALL, ,CLASS, __VA_ARGS__)  \
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adds properly prefixed enum elements.
//
// This is a helper macro to be used in conjunction with metamacro_foreach_cxt when iterating enum constants.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HR_ENUM_ENUM_ELEMENT(Index, Prefix, Name) \
    metamacro_concat(Prefix, Name) = Index,
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adds an element with a given name and value to a dictionary-like declaration.
//
// This is a helper macro to be used in conjunction with metamacro_foreach_cxt when iterating property
// names of the enum class.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HR_ENUM_AS_DICTIONARY_ELEMENT(Value, Name) \
    @ # Name : @(Value),
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adds -asDictionary and +asDictionary methods to the given enum class
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HR_ENUM_AS_DICTIONARY(CLASS, ...)                                                                                       \
@interface CLASS(AsDictionary)                                                                                                  \
+ (NSDictionary *) asDictionary;                                                                                                \
- (NSDictionary *) asDictionary;                                                                                                \
@end                                                                                                                            \
                                                                                                                                \
static id hr_enum ## CLASS ##_method_asDictionary_implementation(id SELF, SEL CMD, ...)                                         \
{                                                                                                                               \
    return [[SELF class] asDictionary];                                                                                         \
}                                                                                                                               \
                                                                                                                                \
static id hr_enum ## CLASS ##_classmethod_asDictionary_implementation(id SELF, SEL CMD, ...)                                    \
{                                                                                                                               \
    return @{ metamacro_foreach(HR_ENUM_AS_DICTIONARY_ELEMENT, , __VA_ARGS__) };                                                \
}                                                                                                                               \
                                                                                                                                \
__attribute__((unused))                                                                                                         \
static void hr_enum_construct_ ## CLASS ##_asDictionary(void)                                                                   \
{                                                                                                                               \
    Class theClass  = objc_getClass(# CLASS);                                                                                   \
    Class metaClass = object_getClass(theClass);                                                                                \
    class_addMethod(theClass,  @selector(asDictionary), &hr_enum ## CLASS ##_method_asDictionary_implementation,      "@@");    \
    class_addMethod(metaClass, @selector(asDictionary), &hr_enum ## CLASS ##_classmethod_asDictionary_implementation, "@@");    \
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

