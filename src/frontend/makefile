##########################################################################
# 
# makefile for the FRONTEND of real-time fMRI system
# just make all subdirs
#
# Alexey Sidorov <scopicsoftware> 2008-10-30
#
##########################################################################

default: front
front: sub_dirs
	$(CC) $(C_FLAGS) -c main.cpp -o $(OBJ_DIR)/main.o

sub_dirs:
	cd ./Actors	&&	$(MAKE)
	cd ./Commands	&&	$(MAKE)
	cd ./Commands/MaskCommands	&&	$(MAKE)
	cd ./Controllers &&	$(MAKE)
	cd ./Document	&&	$(MAKE)
	cd ./Filters	&&	$(MAKE)
	cd ./FrApplication &&	$(MAKE)
	cd ./FrMainWindow  &&	$(MAKE)
	cd ./Layers	&&	$(MAKE)
	cd ./Tools	&&	$(MAKE)
	cd ./Views	&&	$(MAKE)	
	cd ./Utils	&&	$(MAKE)
	cd ./moc	&&	$(MAKE) TYPE=main -f ./makefile.moc
	cd ./moc	&&	$(MAKE) TYPE=util -f ./makefile.moc
	cd ./moc	&&	$(MAKE) TYPE=app -f ./makefile.moc
	cd ./moc	&&	$(MAKE) TYPE=widget -f ./makefile.moc
	cd ./moc	&&	$(MAKE)
	cd ./Widgets	&&	$(MAKE)

clean:
	@$(ECHO) 'make: removing object and autosave files'
	-$(RM) -f *.o *~
	cd ./Actors	&&	$(MAKE) clean
	cd ./Commands	&&	$(MAKE) clean
	cd ./Controllers &&	$(MAKE) clean
	cd ./Document	&&	$(MAKE) clean
	cd ./Filters	&&	$(MAKE) clean
	cd ./FrApplication &&	$(MAKE) clean
	cd ./FrMainWindow  &&	$(MAKE) clean
	cd ./Layers	&&	$(MAKE) clean
	cd ./Tools	&&	$(MAKE) clean
	cd ./Utils	&&	$(MAKE) clean
	cd ./Views	&&	$(MAKE) clean
	cd ./Widgets	&&	$(MAKE) clean
	cd ./moc	&&	$(MAKE) clean
