////////////////////////////////////////////////////////////////////////////////
//
//  AppsQuick.ly
//  Copyright 2013 AppsQuick.ly
//  All Rights Reserved.
//
//  NOTICE: AppsQuick.ly permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////



#import <objc/runtime.h>
#import <objc/message.h>
#import "TyphoonAssembly.h"
#import "TyphoonJRSwizzle.h"
#import "TyphoonDefinition.h"
#import "TyphoonComponentFactory.h"

static NSMutableArray* resolveStack;

@implementation TyphoonAssembly


/* =========================================================== Class Methods ============================================================ */
+ (TyphoonAssembly*)assembly
{
    return [[[self class] alloc] init];
}

+ (BOOL)resolveInstanceMethod:(SEL)sel
{
    if ([TyphoonAssembly selectorReserved:sel])
    {
        return [super resolveInstanceMethod:sel];
    }

    NSString* name = NSStringFromSelector(sel);
    if ([name hasSuffix:TYPHOON_BEFORE_ADVICE_SUFFIX])
    {
        IMP imp = imp_implementationWithBlock((__bridge id) objc_unretainedPointer(^(id me)
        {
            NSString* key = [name stringByReplacingOccurrencesOfString:TYPHOON_BEFORE_ADVICE_SUFFIX withString:@""];
            TyphoonDefinition* cached = [[me cachedSelectors] objectForKey:key];
            if (cached == nil)
            {
                [resolveStack addObject:key];
                if ([resolveStack count] > 2)
                {
                    NSString* bottom = [resolveStack objectAtIndex:0];
                    NSString* top = [resolveStack objectAtIndex:[resolveStack count] - 1];
                    if ([top isEqualToString:bottom])
                    {
                        NSLog(@"Resolve stack: %@", resolveStack);
                        return [[TyphoonDefinition alloc] initWithClass:[NSString class] key:key];
//                        [NSException raise:NSInternalInconsistencyException format:@"Circular dependency detected."];
                    }
                }

                [[self class] typhoon_swizzleMethod:sel withMethod:NSSelectorFromString(key) error:nil];
                cached = objc_msgSend(me, sel);
                if (cached && [cached isKindOfClass:[TyphoonDefinition class]])
                {
                    TyphoonDefinition* definition = (TyphoonDefinition*) cached;
                    if ([definition.key length] == 0)
                    {
                        definition.key = key;
                    }
                    [[me cachedSelectors] setObject:definition forKey:key];
                }
                [[self class] typhoon_swizzleMethod:NSSelectorFromString(key) withMethod:sel error:nil];
            }
            [resolveStack removeAllObjects];
            return cached;

        }));
        class_addMethod(self, sel, imp, "@");
        return YES;
    }
    return NO;
}

+ (TyphoonAssembly*)defaultAssembly
{
    return (TyphoonAssembly*) [TyphoonComponentFactory defaultFactory];
}


+ (BOOL)selectorReserved:(SEL)selector
{
    return selector == @selector(init) || selector == @selector(cachedSelectors) || selector == NSSelectorFromString(@".cxx_destruct") ||
            selector == @selector(defaultAssembly);
}

+ (void)load
{
    [super load];
    resolveStack = [[NSMutableArray alloc] init];
}

/* ============================================================ Initializers ============================================================ */
- (id)init
{
    self = [super init];
    if (self)
    {
        _cachedDefinitions = [[NSMutableDictionary alloc] init];
    }
    return self;
}

/* ============================================================ Utility Methods ========================================================= */
- (void)dealloc
{
    NSLog(@"$$$$$$ %@ in dealloc!", [self class]);
}

/* ============================================================ Private Methods ========================================================= */
- (NSMutableDictionary*)cachedSelectors
{
    return _cachedDefinitions;
}


@end