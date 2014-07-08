deps_config := \
	Kconfig.auto \
	Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
