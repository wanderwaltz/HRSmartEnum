////////////////////////////////////////////////////////////////////////////////
//
//  JASPER BLUES
//  Copyright 2013 Jasper Blues
//  All Rights Reserved.
//
//  NOTICE: Jasper Blues permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////


#import "TyphoonPropertyPlaceholderConfigurer.h"
#import "TyphoonResource.h"
#import "TyphoonDefinition.h"
#import "TyphoonInitializer.h"
#import "TyphoonPropertyInjectedByValue.h"
#import "TyphoonParameterInjectedByValue.h"
#import "TyphoonDefinition+InstanceBuilder.h"
#import "TyphoonInitializer+InstanceBuilder.h"

@interface TyphoonPropertyInjectedByValue (PropertyPlaceHolderConfigurer)

- (void)setTextValue:(NSString*)textValue;

@end


@implementation TyphoonPropertyInjectedByValue (PropertyPlaceHolderConfigurer)

- (void)setTextValue:(NSString*)textValue
{
    _textValue = textValue;
}


@end

@implementation TyphoonPropertyPlaceholderConfigurer

/* =========================================================== Class Methods ============================================================ */
+ (TyphoonPropertyPlaceholderConfigurer*)configurer
{
    return [[[self class] alloc] init];
}

+ (TyphoonPropertyPlaceholderConfigurer*)configurerWithResource:(id <TyphoonResource>)resource
{
    TyphoonPropertyPlaceholderConfigurer* configurer = [TyphoonPropertyPlaceholderConfigurer configurer];
    [configurer usePropertyStyleResource:resource];
    return configurer;
}

+ (TyphoonPropertyPlaceholderConfigurer*)configurerWithResources:(id <TyphoonResource>)first, ...
{
    TyphoonPropertyPlaceholderConfigurer* configurer = [TyphoonPropertyPlaceholderConfigurer configurer];
    [configurer usePropertyStyleResource:first];

    va_list resource_list;
    va_start(resource_list, first);
    id <TyphoonResource> resource;
    while ((resource = va_arg( resource_list, id < TyphoonResource >)))
    {
        [configurer usePropertyStyleResource:resource];
    }
    va_end(resource_list);
    return configurer;
}


/* ============================================================ Initializers ============================================================ */
- (id)initWithPrefix:(NSString*)prefix suffix:(NSString*)suffix
{
    self = [super init];
    if (self)
    {
        _prefix = prefix;
        _suffix = suffix;
        _properties = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (id)init
{
    return [self initWithPrefix:@"${" suffix:@"}"];
}

/* ========================================================== Interface Methods ========================================================= */
- (void)usePropertyStyleResource:(id <TyphoonResource>)resource
{
    NSArray* lines = [[resource asString] componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]];
    
    NSCharacterSet *whitespace = [NSCharacterSet whitespaceCharacterSet];
    
    for (NSString* line in lines)
    {
        if (![line hasPrefix:@"#"])
        {
            NSRange range = [line rangeOfString:@"="];
            if (range.location != NSNotFound)
            {
                NSString* property = [line substringToIndex:range.location];
                NSString* value = [line substringFromIndex:range.location + range.length];
                [_properties setObject: [value    stringByTrimmingCharactersInSet: whitespace]
                                forKey: [property stringByTrimmingCharactersInSet: whitespace]];
            }
        }
    }
}

- (NSDictionary*)properties
{
    return [_properties copy];
}


/* =========================================================== Protocol Methods ========================================================= */
- (void)mutateComponentDefinitionsIfRequired:(NSArray*)componentDefinitions
{
    for (TyphoonDefinition* definition in componentDefinitions)
    {
        for (TyphoonParameterInjectedByValue* parameter in [definition.initializer parametersInjectedByValue])
        {
            if ([parameter.textValue hasPrefix:_prefix] && [parameter.textValue hasSuffix:_suffix])
            {
                NSString* key = [parameter.textValue substringFromIndex:[_prefix length]];
                key = [key substringToIndex:[key length] - [_suffix length]];
                NSString* value = [_properties valueForKey:key];
                NSLog(@"Setting property '%@' to value '%@'", key, value);
                parameter.textValue = value;
            }
        }

        for (TyphoonPropertyInjectedByValue* property in [definition propertiesInjectedByValue])
        {
            if ([property.textValue hasPrefix:_prefix] && [property.textValue hasSuffix:_suffix])
            {
                NSString* key = [property.textValue substringFromIndex:[_prefix length]];
                key = [key substringToIndex:[key length] - [_suffix length]];
                NSString* value = [_properties valueForKey:key];
                NSLog(@"Setting property '%@' to value '%@'", key, value);
                property.textValue = value;
            }
        }
    }
}

@end