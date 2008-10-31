##########################################################################
# 
# makefile for the FRONTEND of real-time fMRI system
# just make all subdirs
#
# Alexey Sidorov <scopicsoftware> 2008-10-30
#
##########################################################################

default:
	$(CC) $(C_FLAGS) -c main.cpp -o $(OBJ_DIR)/main.o
	cd ./Actors	&&	$(MAKE)
	cd ./Commands	&&	$(MAKE)
	cd ./Controllers &&	$(MAKE)
	cd ./Document	&&	$(MAKE)
	cd ./Filters	&&	$(MAKE)
	cd ./FrApplication &&	$(MAKE)
	cd ./FrMainWindow  &&	$(MAKE)
	cd ./Layers	&&	$(MAKE)
	cd ./Tools	&&	$(MAKE)
	cd ./Views	&&	$(MAKE)
	cd ./Widgets	&&	$(MAKE)
	cd ./Utils	&&	$(MAKE)
	cd ./moc	&&	$(MAKE) -f ./makefile.mocing	&& $(MAKE) 	

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
