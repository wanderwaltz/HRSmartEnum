HRSmartEnum
===========

A set of Objective-C macros to declare enums with additional metadata for easier processing.

The `HR_ENUM(...)` macro which is used to declare smart enums.

Smart enum consists of two entities: a plain C enum and an Objective-C class representing the same enum.

This macro declares both of these entities and should be used in header files with the following syntax:
  
      @HR_ENUM(MyEnum,     // First parameter of the macro is the enum name
               Value1, 0, @"String description of the Value1",
               Value2, 1, @"String description of the Value2",
               Value5, 5,, // String description for Value3 is omitted, note the extra comma (,)
               Value6,,);  // String description and explicit value for Value4 is omitted, note the 2 extra commas(,,)

First parameter of the macro declares the enum name. This name will be used as the name of the enum class.
C-style enum declared by this macro will have the name with _t prefix to be easily distinguished from the enum class.

The rest of the parameters declare enum constants and come in threes. Due to the preprocessor-based implementation
you have to provide each of the parameter, but the parameters itself may be empty if you wish to skip some (hence
the extra commas for omitted parameters in the example above).

The parameters are described as following:
- Enum constant name without enum prefix. In our example we're describing an enum named MyEnum. The first parameter
   after the enum name is typed as `Value1` which will yield enum constant `MyEnumValue1`.
- Enum constant explicit value. In our example after Value1 we have parameter 0, which will yield the following
   line after preprocessing: `MyEnumValue1 = 0`
   You can omit explicit values if you wish (keeping the extra comma after the constant name parameter), this can
   be seen in example above for the Value6 constant). The usual enum behavior will apply in this case and `Value6` will
   have the value of `Value5+1`, which indeed equals to 6 in our example.
- String description of the enum constant.

   String descriptions for enum values can be provided and accessed through the enum class. These strings
   could be displayed in the user interface for example if needed. If you don't want to provide descriptions
   for the enum constants, these strings could be omitted (keeping the extra comma in place)

Example above generates a C enum named `MyEnum_t` (suffix _t is added to differentiate between the C enum
and corresponding class) and a class named `MyEnum`.

Constants of the enum will be named `MyEnumValue1`, `MyEnumValue2`, `MyEnumValue5`, `MyEnumValue6` and will have
values 0, 1, 5, 6 correspondingly.
   
For each of the enum value declared the class MyEnum will have the following (take Value1 for example):
* a readonly nonatomic `NSInteger` property named `Value1` which always returns `MyEnumValue1`,
* a class method `+ (NSInteger) Value1` which always returns `MyEnumValue1`

Additionally, MyEnum class will have the following methods:
* `+ (NSDictionary *) asDicitonary` which returns a dictionary representation of the enum in the form of
        `@{ @"Value1" : @(MyEnumValue1), @"Value2" : @(MyEnumValue2), @"Value5" : @(MyEnumValue5), ... }`
* `+ (NSArray *) allKeys` which returns keys array of the dictionary representation
* `+ (NSArray *) allValues` which returns values array of the dictionary representation
* `- (NSDictionary *) asDictionary` - instance method version of the +dictionary
* `- (NSArray *) allKeys` - instance method version of +allKeys
* `- (NSArray *) allValues` - instance method version of +allValues
* Both + and - versions of the `+ (NSDictionary *) descriptionForValue` method which maps the enum constant
   values into the string descriptions which were provided in the HR_ENUM declaration.
   In our example calling `[MyEnum descriptionForValue][@(MyEnumValue1)]` would yield the string
   `@"String description of the Value1"`

Please note that current limitations of the `metamacro_foreach_cxt` macros from extobjc library
allow only 30 different values to be handled by HR_ENUM macro.
(We've expanded extobjc's set of metamacros to support up to 100 arguments, but all our arguments come in
groups of three, so the actual limit is equal to 33; see metamacro_extra_foreach_cxt.h for more information. )

Also note that MyEnum class in the example above is only declared by using `HR_ENUM` macro and should be
defined in one of the .m files in the application. This can be done using `SYNTHESIZE_HR_ENUM` macro which
accepts the enum name as a parameter.
