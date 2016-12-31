OBJS = cd.o exit.o mycat.o mycmp.o mycp.o myenv.o myhead.o myls.o mymkdir.o mymv.o myps.o mypwd.o myrm.o myrmdir.o mytail.o mysort.o main.o

RM := rm -rf

# All Target
all: myshell

# Tool invocations
myshell: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "myshell" $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) myshell
	-@echo ' '

