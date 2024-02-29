#!/bin/bash 
#Used in domain-specific packages to run unit tests

cd `dirname $0`

CLASSPATH=`build-classpath junit judm`:./@DSPACKAGE@.jar:./@DSPACKAGE@-test.jar
export CLASSPATH

java edu.vanderbilt.isis.@DSPACKAGE_LOWER@.test.AllTests
