SUMMARY = "Hardware drivers for ${MACHINE}"
SECTION = "base"
PRIORITY = "required"
LICENSE = "CLOSED"
require conf/license/license-close.inc

PACKAGE_ARCH = "${MACHINE_ARCH}"

RDEPENDS:${PN} = "libjpeg-turbo pulseaudio-lib-rtp"
PROVIDES = "virtual/blindscan-dvbs"

PV = "${KV}+${SRCDATE}"

SRC_URI = "http://source.mynonpublic.com/abcom/${MACHINE}-drivers-${KV}-${SRCDATE}.zip"
SRC_URI:append = " file://suspend.sh"

S = "${WORKDIR}"

INHIBIT_PACKAGE_STRIP = "1"

do_compile() {
}

do_populate_sysroot() {
}

do_install() {
	install -d ${D}${nonarch_base_libdir}/modules/${KV}/extra
	install -d ${D}${sysconfdir}/modules-load.d
	install -m 0755 ${S}/*.ko ${D}${nonarch_base_libdir}/modules/${KV}/extra
	for i in ${MACHINE}_1 ${MACHINE}_2 ${MACHINE}_3 ${MACHINE}_4; do
		echo $i >> ${D}${sysconfdir}/modules-load.d/_${MACHINE}.conf
	done
	install -d ${D}${INIT_D_DIR}
	install -d ${D}${bindir}
	install -m 0755 ${S}/suspend.sh ${D}${INIT_D_DIR}/suspend
	install -m 0755 ${S}/turnoff_power ${D}${bindir}
}

FILES:${PN} += "${sysconfdir}/modules-load.d/_${MACHINE}.conf ${nonarch_base_libdir}/modules/${KV}/extra ${bindir} ${INIT_D_DIR}"

INITSCRIPT_NAME = "suspend"
INITSCRIPT_PARAMS = "start 89 0 ."
inherit update-rc.d

do_package_qa() {
}
