SUMMARY = "${MACHINE} bootargs files"
SECTION = "base"
PRIORITY = "required"
LICENSE = "CLOSED"

PACKAGE_ARCH = "${MACHINE_ARCH}"

PV = "${SRCDATE}"

inherit deploy

SRC_URI = "http://source.mynonpublic.com/abcom/${MACHINE}-bootargs-${SRCDATE}.zip"

S = "${WORKDIR}"

ALLOW_EMPTY:${PN} = "1"
do_configure[nostamp] = "1"

do_deploy() {
	install -d ${DEPLOY_DIR_IMAGE}/abcom-partitions-${MACHINE}/${MACHINE}
	install -m 0755 ${WORKDIR}/bootargs.bin ${DEPLOY_DIR_IMAGE}/abcom-partitions-${MACHINE}
	install -m 0755 ${WORKDIR}/${MACHINE}/bootargs.bin ${DEPLOY_DIR_IMAGE}/abcom-partitions-${MACHINE}/${MACHINE}
}

addtask deploy before do_build after do_install
