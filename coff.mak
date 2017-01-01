COMMONDIR = C:/SaturnOrbit/COMMON
PREFIX = sl_coff

OBJFILE = $(PREFIX).coff
MAPFILE = $(PREFIX).map
LINKFILE = $(COMMONDIR)/sl.lnk
BINFILE = $(PREFIX).bin
ISOFILE = $(PREFIX).iso
IPFILE = $(COMMONDIR)/IP.BIN

all-before:

all-after: $(BINFILE) $(ISOFILE)

$(BINFILE): $(OBJFILE)
	sh-coff-objcopy -O binary $(OBJFILE) $(BINFILE)

$(ISOFILE): $(BINFILE)
	sh-coff-objcopy -O binary $(OBJFILE) cd/0.bin
	mkisofs -quiet -sysid "SEGA SATURN" -volid "SaturnApp" -volset "SaturnApp" -publisher "SEGA ENTERPRISES, LTD." -preparer "SEGA ENTERPRISES, LTD." -appid "SaturnApp" -abstract "ABS.TXT" -copyright "CPY.TXT" -biblio "BIB.TXT" -generic-boot $(IPFILE) -full-iso9660-filenames -o $(ISOFILE) cd  
