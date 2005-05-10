SUBDIRS = src 
EXTRA_DIST = bootstrap 

# For my convenience..
# `make upload' uploads the current version to my website.
upload: distcheck
	-ncftpput -f ~/.ncftp/opts public_html/projects/$(PACKAGE) \
			$(PACKAGE)-$(VERSION).tar.gz


