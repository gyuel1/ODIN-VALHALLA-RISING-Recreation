// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define OD_DATA_ACCESSOR(DataType,PropertyName) \
FORCEINLINE DataType Get##PropertyName() const { return PropertyName;} \
void Set##PropertyName(DataType In##PropertyName) {PropertyName = In##PropertyName;}
