////////////////////////////////////////////////////////////////////////////////
//
//  INFRAXIS
//  Copyright 2013 Infraxis
//  All Rights Reserved.
//
//  NOTICE: Infraxis permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

#import "TyphoonTypeConvertedCollectionValue.h"


@implementation TyphoonTypeConvertedCollectionValue

/* ============================================================ Initializers ============================================================ */
- (id)initWithTextValue:(NSString*)textValue requiredType:(Class)requiredType
{
    self = [super init];
    if (self)
    {
        _textValue = textValue;
        _requiredType = requiredType;
    }
    return self;
}

- (TyphoonCollectionValueType)type
{
    return TyphoonCollectionValueTypeConvertedText;
}

/* ============================================================ Utility Methods ========================================================= */
- (NSString*)description
{
    NSMutableString* description = [NSMutableString stringWithFormat:@"<%@: ", NSStringFromClass([self class])];
    [description appendFormat:@"self.textValue=%@", self.textValue];
    [description appendFormat:@", self.requiredType=%@", self.requiredType];
    [description appendString:@">"];
    return description;
}

@end