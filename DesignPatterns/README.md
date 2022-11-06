 <img align="right" src="arduino_merg_logo.png"  width="150" height="75">

# DesignPatterns
This Library implements the Composite and Observer Design Patterns for use with Arduino ARM code.

Version 0.1.0

Under development.

John Fletcher <john@bunbury28.plus.com>

## Introduction

The Composite and Observer Patterns are well known and described in the Design Patterns book and elsewhere.

These implementations are adapted from various published codes.

The Composite Pattern provide a way of combining objects in Arduino code.

The Observer Pattern can then be use to provide for Observer objects to receive messages when a Subject object changes.

The two patterns are also available combined so that all the objects of Composite are also Subjects for the Observer pattern.

## composite.h

Implements the Composite Pattern

This can be used on its own or with observer as well.

## observer.h 

Implements the Observer Pattern.

This can be used on its own or with composite as well.

## combined.h 

Implements a pattern combining the Composite pattern with the Subjects of the Observer Pattern. It uses observer.h

This header file should be used on its own instead of the other two.

## Usage

See the examples (not there yet).
