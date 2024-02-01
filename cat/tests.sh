#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
TEST_FILE="bytes.txt"
NUMBER=1
echo "" > log.txt

GREEN='\033[32m'
RED='\033[31m'
NORMAL='\033[0m'

for var in -b -e -n -s -t -v 
do
          TEST1="$var $TEST_FILE"
          ./s21_cat $TEST1 > s21_cat.txt
          cat $TEST1 > cat.txt
          DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
          if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
            then
              echo -e $NORMAL TEST $NUMBER "$TEST1" $GREEN "\tSUCCESS"
              (( COUNTER_SUCCESS++ ))
            else
              echo -e $NORMAL TEST $NUMBER "$TEST1" $RED "\tFAIL"
              echo "$TEST1" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          ((NUMBER++))
          rm s21_cat.txt cat.txt
done

for var in -b -e -n -s -t -v -e
do
  for var2 in -b -e -n -s -t -v -b
  do
        if [ $var != $var2 ]
        then
          TEST1="$var $var2 $TEST_FILE"
          ./s21_cat $TEST1 > s21_cat.txt
          cat $TEST1 > cat.txt
          DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
          if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
            then
              echo -e $NORMAL TEST $NUMBER "$TEST1" $GREEN "\tSUCCESS"
              (( COUNTER_SUCCESS++ ))
            else
              echo -e $NORMAL TEST $NUMBER "$TEST1" $RED "\tFAIL"
              echo "$TEST1" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          ((NUMBER++))
          rm s21_cat.txt cat.txt
        fi
  done
done

for var in -b -e -n -s -t -v 
do
  for var2 in -b -e -n -s -t -v 
  do
      for var3 in -b -e -n -s -t -v 
      do
        if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
        then
          TEST1="$var $var2 $var3 $TEST_FILE"
          ./s21_cat $TEST1 > s21_cat.txt
          cat $TEST1 > cat.txt
          DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
          if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
            then
              echo -e $NORMAL TEST $NUMBER "$TEST1" $GREEN "\tSUCCESS"
              (( COUNTER_SUCCESS++ ))
            else
              echo -e $NORMAL TEST $NUMBER "$TEST1" $RED "\tFAIL"
              echo "$TEST1" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          ((NUMBER++))
          rm s21_cat.txt cat.txt
        fi
      done
  done
done

for var in -b -e -n -s -t -v 
do
  for var2 in -b -e -n -s -t -v 
  do
      for var3 in -b -e -n -s -t -v 
      do
          for var4 in -b -e -n -s -t -v 
          do
            if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ] && [ $var != $var4 ] && [ $var2 != $var4 ] && [ $var3 != $var4 ]
            then
              TEST1="$var $var2 $var3 $var4 $TEST_FILE"
              ./s21_cat $TEST1 > s21_cat.txt
              cat $TEST1 > cat.txt
              DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
              if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
                then
                  echo -e $NORMAL TEST $NUMBER "$TEST1" $GREEN "\tSUCCESS"
                  (( COUNTER_SUCCESS++ ))
                else
                  echo -e $NORMAL TEST $NUMBER "$TEST1" $RED "\tFAIL"
                  echo "$TEST1" >> log.txt
                  (( COUNTER_FAIL++ ))
              fi
              ((NUMBER++))
              rm s21_cat.txt cat.txt
            fi
          done
      done
  done
done

echo -e $GREEN SUCCESS $COUNTER_SUCCESS
echo -e $RED FAIL $COUNTER_FAIL