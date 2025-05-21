################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/ALLvariable.c \
../code/Communication.c \
../code/Control.c \
../code/ExternalDevice.c \
../code/Filter.c \
../code/Init.c \
../code/Menu.c \
../code/MyFlash.c \
../code/TrackInformation.c 

COMPILED_SRCS += \
./code/ALLvariable.src \
./code/Communication.src \
./code/Control.src \
./code/ExternalDevice.src \
./code/Filter.src \
./code/Init.src \
./code/Menu.src \
./code/MyFlash.src \
./code/TrackInformation.src 

C_DEPS += \
./code/ALLvariable.d \
./code/Communication.d \
./code/Control.d \
./code/ExternalDevice.d \
./code/Filter.d \
./code/Init.d \
./code/Menu.d \
./code/MyFlash.d \
./code/TrackInformation.d 

OBJS += \
./code/ALLvariable.o \
./code/Communication.o \
./code/Control.o \
./code/ExternalDevice.o \
./code/Filter.o \
./code/Init.o \
./code/Menu.o \
./code/MyFlash.o \
./code/TrackInformation.o 


# Each subdirectory must supply rules for building sources it contributes
code/%.src: ../code/%.c code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/adsworkspace/MyCar/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

code/%.o: ./code/%.src code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-code

clean-code:
	-$(RM) ./code/ALLvariable.d ./code/ALLvariable.o ./code/ALLvariable.src ./code/Communication.d ./code/Communication.o ./code/Communication.src ./code/Control.d ./code/Control.o ./code/Control.src ./code/ExternalDevice.d ./code/ExternalDevice.o ./code/ExternalDevice.src ./code/Filter.d ./code/Filter.o ./code/Filter.src ./code/Init.d ./code/Init.o ./code/Init.src ./code/Menu.d ./code/Menu.o ./code/Menu.src ./code/MyFlash.d ./code/MyFlash.o ./code/MyFlash.src ./code/TrackInformation.d ./code/TrackInformation.o ./code/TrackInformation.src

.PHONY: clean-code

