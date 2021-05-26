# We know about rtc
MACHINE_FEATURES_BACKFILL_CONSIDERED = "rtc"

BOX_BRAND = "abcom"

KERNELVERSION = "4.4.35"

require conf/machine/include/soc-family.inc

require conf/machine/include/tune-cortexa15.inc

MACHINE_FEATURES += "alsa hdmicec switchoff blindscan-tbs hisil-${HICHIPSET} hisil emmc mali wol 7segment multitranscoding blindscan-dvbs dvb-c ci fastboot mountboot HDMI kodi19"

SOC_FAMILY = "hisi3798mv200"
HICHIPSET = "3798mv200"
CHIPSET = "Hi3798Mv200"

EXTRA_OECONF_append_pn-enigma2 = " --with-alphablendingacceleration=always --with-blitaccelerationthreshold=250  --with-fillaccelerationthreshold=190000"

DVBMEDIASINK_CONFIG = "--with-wma --with-wmv --with-pcm --with-dts --with-eac3 --with-h265 --with-vb6 --with-vb8 --with-vb9 --with-spark"

PREFERRED_PROVIDER_virtual/egl ?= "abcom-mali-${MACHINE}"
PREFERRED_PROVIDER_virtual/libgles1 ?= "abcom-mali-${MACHINE}"
PREFERRED_PROVIDER_virtual/libgles2 ?= "abcom-mali-${MACHINE}"
PREFERRED_PROVIDER_virtual/libgbm ?= "abcom-mali-${MACHINE}"
PREFERRED_PROVIDER_virtual/libgl ?= "mesa"
PREFERRED_PROVIDER_virtual/mesa ?= "mesa"

PREFERRED_PROVIDER_virtual/blindscan-dvbc = "abcom-dvb-modules-${MACHINE}"
PREFERRED_PROVIDER_virtual/blindscan-dvbs = "abcom-dvb-modules-${MACHINE}"

PREFERRED_PROVIDER_virtual/kernel = "linux-abcom"
PREFERRED_VERSION_linux-abcom = "${KERNELVERSION}"
PREFERRED_PROVIDER_virtual/enigma2-mediaservice = "enigma2-plugin-systemplugins-servicehisilicon"

MTD_BOOTFS = "mmcblk0p4"
MTD_ROOTFS = "mmcblk0p21"
MTD_KERNEL = "mmcblk0p20"
MTD_BLACK  = "mmcblk0"
KERNEL_FILE = "uImage"
ROOTFS_FILE = "rootfs.tar.bz2"

TARGET_ARCH = "arm"
DEFAULTTUNE = "cortexa15hf-neon-vfpv4"

MACHINE_ESSENTIAL_EXTRA_RDEPENDS += "\
	abcom-libs-${MACHINE} \
	abcom-fastboot-${MACHINE} \
	abcom-param-${MACHINE} \
	abcom-bootlogo-${MACHINE} \
	abcom-bootargs-${MACHINE} \
	abcom-recovery-${MACHINE} \
	abcom-dvb-modules-${MACHINE} \
	${@bb.utils.contains('MACHINE_FEATURES', 'mali', 'abcom-mali-${MACHINE} kernel-module-mali-${MACHINE}' , '', d)} \
	${@bb.utils.contains('MACHINE_FEATURES', 'wol', 'abcom-ethwol' , '', d)} \
	${@bb.utils.contains('MACHINE_FEATURES', 'emmc', 'abcom-resizerootfs partitions-by-name' , '', d)} \
	"

CMDLINE ?= "console=ttyAMA0,115200n8"

KERNEL_MODULE_AUTOLOAD += "mali"

IMAGE_CLASSES += " abemmc-fastboot8gb_image"
IMAGE_FSTYPES += " abfastboot8gb"

IMAGE_CMD_abfastboot8gb_append = " \
	mkdir -p ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/${IMAGEDIR}; \
	cp ${IMGDEPLOYDIR}/bootoptions.img ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/${IMAGEDIR}/bootoptions.img; \
	cp -f ${IMGDEPLOYDIR}/misc-boot.img ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/${IMAGEDIR}/misc-boot.img; \
	cp -f ${DEPLOY_DIR_IMAGE}/abcom-partitions-${MACHINE}/fastboot.bin ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/fastboot.bin; \
	cp -f ${DEPLOY_DIR_IMAGE}/abcom-partitions-${MACHINE}/bootargs.bin ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/bootargs.bin; \
	cp -f ${DEPLOY_DIR_IMAGE}/abcom-partitions-${MACHINE}/${MACHINE}/fastboot.bin ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/${IMAGEDIR}/fastboot.bin; \
	cp -f ${DEPLOY_DIR_IMAGE}/abcom-partitions-${MACHINE}/${MACHINE}/bootargs.bin ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/${IMAGEDIR}/bootargs.bin; \
	cp -f ${DEPLOY_DIR_IMAGE}/abcom-partitions-${MACHINE}/${MACHINE}/pqparam.bin ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/${IMAGEDIR}/pqparam.bin; \
	cp -f ${DEPLOY_DIR_IMAGE}/abcom-partitions-${MACHINE}/${MACHINE}/baseparam.img ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/${IMAGEDIR}/baseparam.img; \
	if [ -e ${DEPLOY_DIR_IMAGE}/abcom-partitions-${MACHINE}/${MACHINE}/recovery.bin ]; then \
		cp -f ${DEPLOY_DIR_IMAGE}/abcom-partitions-${MACHINE}/${MACHINE}/recovery.bin ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/${IMAGEDIR}/recovery.bin; \
	fi; \
	cp -f ${DEPLOY_DIR_IMAGE}/abcom-partitions-${MACHINE}/${MACHINE}/logo.img ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/${IMAGEDIR}/logo.img; \
	echo "${MACHINE}-${DATE}-RECOVERY" > ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/${IMAGEDIR}/recoveryversion; \
	echo "***** ATTENTION *****" >${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_READ.ME; \
	echo "This RECOVERY only need when you have issue access the MULTIBOOT MENU" >>${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_READ.ME; \
	echo "The ${IMAGE_NAME}_recovery_usb.zip should been install normally once (or if there is an update.)" >>${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_READ.ME; \
	echo "This is needed for avoid images conflicts as the partition layout (bootargs) is of huge essential." >>${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/recovery_${MACHINEBUILD}_READ.ME; \
	echo "A small change can destroy all your install images. So we can better leave it and not install it if it's not need." >>${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/recovery_${MACHINE}_READ.ME; \
	echo "https://forum.openvision.tech/app.php/donate" > ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}/donate.txt; \
	rm -f ${DEPLOY_DIR_IMAGE}/*.zip; \
	cd ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY} ; \
	zip -r ../${IMAGE_NAME}_recovery_emmc.zip *; \
	rm -Rf ${DEPLOY_DIR_IMAGE}/${IMAGEDIRRECOVERY}; \
	mkdir -p ${DEPLOY_DIR_IMAGE}/${MACHINE}/${IMAGEDIR}; \
	cp -f ${DEPLOY_DIR_IMAGE}/uImage ${DEPLOY_DIR_IMAGE}/${MACHINE}/${IMAGEDIR}/uImage; \
	tar -cvf ${DEPLOY_DIR_IMAGE}/rootfs.tar -C ${IMAGE_ROOTFS} .; \
	mv -f ${DEPLOY_DIR_IMAGE}/rootfs.tar ${DEPLOY_DIR_IMAGE}/${MACHINE}/${IMAGEDIR}/rootfs.tar; \
	bzip2 ${DEPLOY_DIR_IMAGE}/${MACHINE}/${IMAGEDIR}/rootfs.tar; \
	echo "${IMAGE_NAME}" > ${DEPLOY_DIR_IMAGE}/${MACHINE}/${IMAGEDIR}/imageversion; \
	echo "https://forum.openvision.tech/app.php/donate" > ${DEPLOY_DIR_IMAGE}/${MACHINE}/${IMAGEDIR}/donate.txt; \
	echo "${IMAGE_NAME}_usb.zip" > ${DEPLOY_DIR_IMAGE}/${MACHINE}/unforce_${MACHINE}.txt; \
	echo "Rename the unforce_${MACHINE}.txt to force_${MACHINE}.txt and move it to the root of your usb-stick" > ${DEPLOY_DIR_IMAGE}/${MACHINE}/${IMAGEDIR}/force_${MACHINE}_READ.ME; \
	echo "When you enter the recovery menu then it will force to install the image ${IMAGE_NAME}_usb.zip in the linux1 selection" >> ${DEPLOY_DIR_IMAGE}/${MACHINE}/${IMAGEDIR}/force_${MACHINE}_READ.ME; \
	cd ${DEPLOY_DIR_IMAGE}/${MACHINE}; \
	zip -r ../${IMAGE_NAME}_usb.zip *; \
	cd ..; \
	rm -Rf ${DEPLOY_DIR_IMAGE}/${MACHINE}; \
	rm -Rf ${DEPLOY_DIR_IMAGE}/abcom-partitions-${MACHINE}; \
	"

IMAGE_CMD_prepend = "\
	rm -Rf ${IMAGE_ROOTFS}/tmp/*; \
	"

EXTRA_ALSA = "mv200"

RCNAME = "abcom"

DISPLAY_BRAND = "AB-COM"
