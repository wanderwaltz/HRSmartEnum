//
//  main.m
//  HRSmartEnum
//
//  Created by Egor Chiglintsev on 10.11.13.
//  Copyright (c) 2013 Sibers. All rights reserved.
//  Released under the MIT license.
//

#import <Foundation/Foundation.h>
#import "HRSmartEnum.h"


// Using HR_ENUM macro you declare both a plain C enum with the given name
// and a class of the same name which allows more high-level operations with
// the enum values (such as enumeration in a for-in loop for example).
//
// Enum is declared with NSInteger type. Current limitations allow only
// enums without explicit values (i.e. the values are assigned automatically,
// starting from 0 and so forth).
@HR_ENUM(MahBoi, // First parameter is the name of the enum
         
         // The rest of the parameters come in pairs - one is the enum constant
         // name which is no different from the usual enum declarations.
         //
         // The second is the string name for this constant which can be displayed
         // in the user interface for example.
         //
         // String name may be omitted, but in order for the macro to expand properly,
         // an extra comma (,) should be then left after the enum constant name.
         Value0, @"String value 1",
         Value1, @"String value 2",
         Value2,,
         Value3,,
         Value4,,
         Value5, @"And here we have another user-friendly string for example",
         Value6,,
         Value7,,
         Value8,,
         Value9,
         // No more than 10 different values can be handled here,
         // this is a limitation of metamacro_foreach and similar macros
         // from the extobjc library. Theoretically we could expand this
         // to any reasonable number of arguments if necessary.
         //
         // In fact, extobjc limits us to 20 values, but we have mandatory
         // parameter pairs here, so we are left with 10 maximum for now.
         );



// Since usually HR_ENUM macro would be used in a header file, we cannot
// place @implementation of the enum class right there in a single macro,
// so we've had to create a separate SYNTHESIZE_HR_ENUM macro which has
// to be used in one of your .m files.
@SYNTHESIZE_HR_ENUM(MahBoi);


int main(int argc, const char * argv[])
{
    @autoreleasepool {
        
        // Not necessary to create instances of this class since
        // it does not contain meaningful methods (only readonly
        // properties), but if you want, it is entirely possible.
        MahBoi *boi = [MahBoi new];
        
        
        // Plain old C enum has suffix _t added to it so we could
        // differentiate it from the enum class.
        //
        // HR_ENUM declares enum with NSInteger type.
        MahBoi_t scalar = MahBoiValue2;

        // You can use scalar enum values in switch statements as usual.
        //
        // You should also have a compiler warning here complainig about
        // not all enum values handled in switch.
        switch (scalar)
        {
            case MahBoiValue0: break;
            case MahBoiValue1: break;
        }
        
        // The constants have values starting from 0 and so forth
        NSLog(@"MahBoiValue0 = %ld", MahBoiValue0);
        NSLog(@"MahBoiValue1 = %ld", MahBoiValue1);
        
        // They can be accessed as class methods of the enum class
        NSLog(@"MahBoi.Value0 = %ld", MahBoi.Value0);
        NSLog(@"MahBoi.Value1 = %ld", MahBoi.Value1);
        
        // And also as readonly properties of the enum class instance
        NSLog(@"boi.Value0 = %ld", boi.Value0);
        NSLog(@"boi.Value1 = %ld", boi.Value1);
        
        // All key-value pairs are accessible as a single dictionary
        // if you want.
        NSLog(@"MahBoi.asDictionary = %@", MahBoi.asDictionary);
        NSLog(@"boi.asDictionary = %@", boi.asDictionary);
        
        // Also convenience methods are provided to get all keys as an NSArray
        NSLog(@"MahBoi.allKeys = %@", MahBoi.allKeys);
        NSLog(@"boi.allKeys     = %@", boi.allKeys);
        
        // And all values too!
        NSLog(@"MahBoi.allValues = %@", MahBoi.allValues);
        NSLog(@"boi.allValues = %@", boi.allValues);
        
        
        // A dictionary which maps enum values into string values
        // is also available as a class and instance methods
        NSLog(@"MahBoi.nameForValue = %@", MahBoi.descriptionForValue);
        NSLog(@"boi.nameForValue = %@", boi.descriptionForValue);
    }
    
    return 0;
}

