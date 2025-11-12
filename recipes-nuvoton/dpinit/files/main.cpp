#include <iostream>
#include <fstream>
// #include <filesystem>
// #include <openbmc/libobmci2c.hpp>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>

/* For i2c API */
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <iostream>
#include <vector>

#include "anx9807.h"

#define SLAVE_ADDR38 0x38
#define SLAVE_ADDR39 0x39
#define SLAVE_ADDR3D 0x3D
// #define I2C_BUS_NUMBER   16
#define SP_TX_PORT0_ADDR 0x38 // 0x70
// #define SP_TX_PORT1_ADDR 0x74
#define SP_TX_PORT2_ADDR 0x39 // 0x72

static uint8_t I2C_BUS_NUMBER = 3;

// #define debug_printf printf
#define debug_printf
#define debug_puts(x) \
	{                 \
		printf(x);    \
		printf("\n"); \
	}

char *edid_path = NULL;
typedef uint8_t BYTE;
bool EDID_Print_Enable = true;
int edid_fd;

int open_i2c_dev(int i2cbus, char *filename, size_t size, int quiet)
{
	int file;

	snprintf(filename, size, "/dev/i2c/%d", i2cbus);
	filename[size - 1] = '\0';
	file = open(filename, O_RDWR);

	if ((file < 0) && (errno == ENOENT || errno == ENOTDIR))
	{
		sprintf(filename, "/dev/i2c-%d", i2cbus);
		file = open(filename, O_RDWR);
	}

	if ((file < 0) && (!quiet))
	{
		if (errno == ENOENT)
		{
			fprintf(stderr, "Error: Could not open file "
							"`/dev/i2c-%d' or `/dev/i2c/%d': %s\n",
					i2cbus, i2cbus, strerror(ENOENT));
		}
		else
		{
			fprintf(stderr, "Error: Could not open file "
							"`%s': %s\n",
					filename, strerror(errno));
			if (errno == EACCES)
				fprintf(stderr, "Run as root?\n");
		}
	}

	return file;
}

void close_i2c_dev(int file)
{
	close(file);
}

int i2c_master_write(int file, uint8_t slave_addr,
					 int tx_cnt, uint8_t *tx_buf)
{
	struct i2c_rdwr_ioctl_data iomsg;
	struct i2c_msg i2cmsg;
	int ret;

	i2cmsg.addr = slave_addr & 0xFF;
	i2cmsg.flags = 0;
	i2cmsg.len = tx_cnt;
	i2cmsg.buf = (uint8_t *)tx_buf;

	iomsg.msgs = &i2cmsg;
	iomsg.nmsgs = 1;

	ret = ioctl(file, I2C_RDWR, &iomsg);

	return ret;
}

int i2c_master_write_read(int file, uint8_t slave_addr,
						  int tx_cnt, uint8_t *tx_buf,
						  int rx_cnt, uint8_t *rx_buf)
{
	struct i2c_rdwr_ioctl_data iomsg;
	struct i2c_msg i2cmsg[2];
	int ret;
	int n_msg = 0;

	if (tx_cnt)
	{
		i2cmsg[n_msg].addr = slave_addr & 0xFF;
		i2cmsg[n_msg].flags = 0;
		i2cmsg[n_msg].len = tx_cnt;
		i2cmsg[n_msg].buf = (uint8_t *)tx_buf;
		n_msg++;
	}

	if (rx_cnt)
	{
		i2cmsg[n_msg].addr = slave_addr & 0xFF;
		i2cmsg[n_msg].flags = I2C_M_RD;
		i2cmsg[n_msg].len = rx_cnt;
		i2cmsg[n_msg].buf = (uint8_t *)rx_buf;
		n_msg++;
	}

	iomsg.msgs = i2cmsg;
	iomsg.nmsgs = n_msg;

	ret = ioctl(file, I2C_RDWR, &iomsg);

	return ret;
}

int SP_TX_Write_Reg(uint8_t dev_addr, uint8_t offset, uint8_t d)
{
	std::vector<char> filename;
	filename.assign(20, 0);
	std::vector<uint8_t> w_cmds;
	int res = -1;
	int fd = open_i2c_dev(I2C_BUS_NUMBER, filename.data(), filename.size(), 0);
	if (fd < 0)
	{
		std::cerr << "Fail to open I2C device: " << I2C_BUS_NUMBER << "\n";
		return -1;
	}

	w_cmds.assign({offset, d});
	res = i2c_master_write(fd, dev_addr, 2, w_cmds.data());
	if (res < 0)
	{
		std::cerr << "Fail to w I2C device: " << I2C_BUS_NUMBER
				  << ", Addr: " << dev_addr << "\n";
		close_i2c_dev(fd);
		return -1;
	}

	close_i2c_dev(fd);
	return 1;
}

uint8_t SP_TX_Read_Reg(uint8_t dev_addr, uint8_t offset, uint8_t *d)
{
	std::vector<char> filename;
	filename.assign(20, 0);
	std::vector<uint8_t> w_cmds;
	int res = -1;
	int fd = open_i2c_dev(I2C_BUS_NUMBER, filename.data(), filename.size(), 0);
	if (fd < 0)
	{
		std::cerr << "Fail to open I2C device: " << I2C_BUS_NUMBER << "\n";
		return -1;
	}

	w_cmds.assign(1, offset);
	res = i2c_master_write_read(fd, dev_addr, 1, w_cmds.data(), 1, d);
	if (res < 0)
	{
		std::cerr << "Fail to w I2C device: " << I2C_BUS_NUMBER
				  << ", Addr: " << dev_addr << "\n";
		close_i2c_dev(fd);
		return -1;
	}

	close_i2c_dev(fd);
	return 1;
}

#define PAGE_SIZE 0x1000
#define NPCM_CLK_BASE 0xF0801000
#define NPCM_CLK_OFFSET_SWRSTC1 0x44
#define NPCM_CLK_GPIO_MASK 0xFFF9FFFF // [18:17] = 00b for GPIOM2 and GPIOM1

int dpinit_reset_control(void)
{
	int32_t mem_fd = 0, val = 0;
	uint8_t *reg_base;
	uint32_t reg_offset;

	/* Clear the reset control register for RST_BMC_DP_N and DP_PWR_EN
	 *   RST_BMC_DP_N : gpio47 = gpio 1 15
	 *   DP_PWR_EN : gpio71 = gpio 2 7
	 */
	mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (mem_fd >= 0)
	{
		reg_base = (uint8_t *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, NPCM_CLK_BASE);
		reg_offset = NPCM_CLK_OFFSET_SWRSTC1;
		if (reg_base == MAP_FAILED)
		{
			std::cerr << __func__ << ": Error mapping 0x" << std::hex << NPCM_CLK_BASE << "\n";
			close(mem_fd);
			return -1;
		}
		val = *((uint32_t *)(reg_base + reg_offset)) & NPCM_CLK_GPIO_MASK;
		*((uint32_t *)(reg_base + reg_offset)) = val;
		munmap(reg_base, PAGE_SIZE);
		close(mem_fd);
		return 0;
	}
	else
	{
		std::cerr << __func__ << ": Error opening /dev/mem\n";
		return -1;
	}
}

void SP_TX_RST_AUX(void)
{
	BYTE c;
	SP_TX_Read_Reg(SP_TX_PORT2_ADDR, SP_TX_RST_CTRL2_REG, &c);
	SP_TX_Write_Reg(SP_TX_PORT2_ADDR, SP_TX_RST_CTRL2_REG, c | SP_TX_AUX_RST);
	SP_TX_Write_Reg(SP_TX_PORT2_ADDR, SP_TX_RST_CTRL2_REG, c & (~SP_TX_AUX_RST));
}
void SP_TX_AddrOnly_Set(bool bSet)
{
	BYTE c;

	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	if (bSet)
	{
		SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, (c | SP_TX_ADDR_ONLY_BIT));
	}
	else
	{
		SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, (c & ~SP_TX_ADDR_ONLY_BIT));
	}
}

void SP_TX_Wait_AUX_Finished(void)
{
	BYTE c;
	BYTE cCnt;
	cCnt = 0;

	// SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_STATUS, &c);
	// while(c & 0x10)
	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	while (c & 0x1)
	{
		cCnt++;

		// SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_STATUS, &c);
		SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);

		if (cCnt > 100)
		{
			printf("AUX Operaton does not finished, and time out.\n");
			break;
		}
	}
}

void SP_TX_AuxOverI2C_Start(void)
{
	BYTE c;

	// set I2C write com 0x04 mot = 1
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG, 0x04);
	// Set the address only bit
	SP_TX_AddrOnly_Set(1);
	// enable aux
	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, c | 0x01);
	SP_TX_Wait_AUX_Finished();
}

void SP_TX_AuxOverI2C_ReStart(void)
{
	BYTE c;

	// set I2C write com 0x04 mot = 1
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG, 0x05);
	// Set the address only bit
	SP_TX_AddrOnly_Set(1);
	// enable aux
	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, c | 0x01);
	SP_TX_Wait_AUX_Finished();
}

void SP_TX_AuxOverI2C_End(void)
{
	BYTE c;

	// set I2C write com 0x04 mot = 1
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG, 0x01);
	// Set the address only bit
	SP_TX_AddrOnly_Set(1);
	// enable aux
	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, c | 0x01);
	SP_TX_Wait_AUX_Finished();
}
void SP_TX_AUX_WR(BYTE offset)
{
	BYTE c, cnt;
	cnt = 0;
	// load offset to fifo
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_BUF_DATA_0_REG, offset);
	// set I2C write com 0x04 mot = 1
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG, 0x04);
	// clear the address only bit
	SP_TX_AddrOnly_Set(0);
	// enable aux
	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, c | 0x01);
	// SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, 0x09);

	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	while (c & 0x01)
	{
		usleep(10000);
		cnt++;
		// debug_printf("cntwr = %.2x\n",(WORD)cnt);
		if (cnt == 10)
		{
			printf("write break");
			SP_TX_RST_AUX();
			cnt = 0;
			break;
		}
		SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	}
}

void SP_TX_AUX_RD(BYTE len_cmd)
{
	BYTE c, cnt;
	cnt = 0;

	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG, len_cmd);
	// Clear  the address only bit
	SP_TX_AddrOnly_Set(0);
	// enable aux
	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, c | 0x01);
	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	while (c & 0x01)
	{
		usleep(10000);
		cnt++;
		// debug_printf("cntrd = %.2x\n",(WORD)cnt);
		if (cnt == 10)
		{
			printf("read break");
			SP_TX_RST_AUX();
			break;
		}
		SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	}
}

BYTE SP_TX_AUX_EDIDRead_Byte(BYTE offset)
{
	/* begin 20130128 for DP link CTS 4.3.2.5 */
	BYTE c, i, edid[16], data_cnt, cnt, vsdbdata[4];
	int VSDBaddr;
	BYTE bReturn = 0;
	BYTE checksum;
	int sp_tx_ds_edid_hdmi = 0;
	static BYTE EDIDExtBlock[128];
	BYTE DTDbeginAddr;

	/* 20130128 for DP link CTS 4.3.2.5 end */

	cnt = 0;
	SP_TX_AuxOverI2C_Start();
	SP_TX_AUX_WR(offset); // offset

	SP_TX_AuxOverI2C_ReStart();

	if ((offset == 0x00) || (offset == 0x80))
		checksum = 0;
	SP_TX_AUX_RD(0xf5); // set I2C read com 0x05 mot = 1 and read 16 bytes

	data_cnt = 0;
	while (data_cnt < 16)
	{
		SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_BUF_DATA_COUNT_REG, &c);
		c = c & 0x1f;
		// debug_printf("cnt_d = %.2x\n",(WORD)c);
		if (c != 0)
		{
			for (i = 0; i < c; i++)
			{
				SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_BUF_DATA_0_REG + i, &edid[i + data_cnt]);
				// debug_printf("edid[%.2x] = %.2x\n",(WORD)(i + offset),(WORD)edid[i + data_cnt]);
				checksum = checksum + edid[i + data_cnt];
			}
		}
		else
		{
			SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG, 0x01);
			// enable aux
			SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
			SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, c | 0x03); // set address only
			SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
			while (c & 0x01)
			{
				usleep(2000);
				cnt++;
				if (cnt == 10)
				{
					// debug_puts("read break");
					SP_TX_RST_AUX();
					bReturn = 0x01;
				}
				SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
			}
			// debug_puts("cnt_d = 0, break");
			bReturn = 0x02; // for fixing bug leading to dead lock in loop "while(data_cnt < 16)"
			return bReturn;
		}
		data_cnt = data_cnt + c;
		if (data_cnt < 16) // 080610. solution for handle case ACK + M byte
		{
			// SP_TX_AUX_WR(offset);
			SP_TX_RST_AUX();
			usleep(10000);

			c = 0x05 | ((0x0f - data_cnt) << 4); // Read MOT = 1
			SP_TX_AUX_RD(c);
			// debug_puts("M < 16");
		}
	}

	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG, 0x01);
	// enable aux
	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, c | 0x03); // set address only to stop EDID reading
	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	while (c & 0x01)
		SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);

	// debug_printf("***************************offset %.2x reading completed\n", (unsigned int)offset);

	if (write(edid_fd, edid, 16) == -1)
		perror("write edid.bin error\n");
	if (EDID_Print_Enable)
	{
		for (i = 0; i < 16; i++)
		{
			if ((i & 0x0f) == 0)
				debug_printf("\n edid: [%.2x]  %.2x  ", (unsigned int)offset, (unsigned int)edid[i]);
			else
				debug_printf("%.2x  ", (unsigned int)edid[i]);

			if ((i & 0x0f) == 0x0f)
				debug_printf("\n");
		}
	}

	if (offset == 0x00)
	{
		if ((edid[0] == 0) && (edid[7] == 0) && (edid[1] == 0xff) && (edid[2] == 0xff) && (edid[3] == 0xff) && (edid[4] == 0xff) && (edid[5] == 0xff) && (edid[6] == 0xff))
		{
			// debug_puts("Good EDID header!");
		}
		else
		{
			debug_puts("Bad EDID header!");
		}
	}

	else if (offset == 0x30)
	{
		// for(i = 0; i < 10; i ++ )
		//	SP_TX_EDID_PREFERRED[i] = edid[i + 6];//edid[0x36]~edid[0x3f]
	}

	else if (offset == 0x40)
	{
		// for(i = 0; i < 8; i ++ )
		//	SP_TX_EDID_PREFERRED[10 + i] = edid[i];//edid[0x40]~edid[0x47]
	}

	else if (offset == 0x70)
	{
		checksum = checksum - edid[15];
		checksum = ~checksum + 1;
		if (checksum != edid[15])
		{
			// debug_puts("Bad EDID check sum1!");
			checksum = edid[15];
			/* begin 20130128 for DP link CTS 4.2.2.6 */
			bReturn = 0x01;
			/* 20130128 for DP link CTS 4.2.2.6 end */
		}
		else
			debug_puts("Good EDID check sum1!");
	}
	else if ((offset >= 0x80) && (sp_tx_ds_edid_hdmi == 0))
	{
		for (i = 0; i < 16; i++) // record all 128 data in extsion block.
			EDIDExtBlock[offset - 0x80 + i] = edid[i];

		if (offset == 0x80)
			DTDbeginAddr = edid[2];

		if (offset == 0xf0)
		{
			checksum = checksum - edid[15];
			checksum = ~checksum + 1;
			if (checksum != edid[15])
			{
				debug_puts("Bad EDID check sum2!");
				/* begin 20130118 for DP link CTS */
				bReturn = 0x01;
				return bReturn;
				/* 20130118 for DP link CTS end */
			}
			else
			{
				debug_puts("Good EDID check sum2!");
			}
			for (VSDBaddr = 0x04; VSDBaddr < DTDbeginAddr;)
			{
				// debug_printf("####VSDBaddr = %.2x\n",(WORD)(VSDBaddr+0x80));

				vsdbdata[0] = EDIDExtBlock[VSDBaddr];
				vsdbdata[1] = EDIDExtBlock[VSDBaddr + 1];
				vsdbdata[2] = EDIDExtBlock[VSDBaddr + 2];
				vsdbdata[3] = EDIDExtBlock[VSDBaddr + 3];

				// debug_printf("vsdbdata= %.2x,%.2x,%.2x,%.2x\n",(WORD)vsdbdata[0],(WORD)vsdbdata[1],(WORD)vsdbdata[2],(WORD)vsdbdata[3]);
				if ((vsdbdata[0] & 0xe0) == 0x60)
				{
					if ((vsdbdata[1] == 0x03) && (vsdbdata[2] == 0x0c) && (vsdbdata[3] == 0x00))
					{
						sp_tx_ds_edid_hdmi = 1;
						return 0;
					}
					else
					{
						sp_tx_ds_edid_hdmi = 0;
						return 0x03;
					}
				}
				else
				{
					sp_tx_ds_edid_hdmi = 0;
					VSDBaddr = VSDBaddr + (vsdbdata[0] & 0x1f);
					VSDBaddr = VSDBaddr + 0x01;
				}
				/* begin 20130128 for DP link CTS 4.3.2.5 */
				if (VSDBaddr > DTDbeginAddr)
					return 0x03;
				/* 20130128 for DP link CTS 4.3.2.5 end */
			}
		}
	}

	return bReturn;
}

int dump_edid(void)
{
	char bEDID_Block;
	uint8_t c, bReturn;
	int i;

	edid_fd = open(edid_path, O_RDWR | O_CREAT, 0644);
	if (edid_fd == -1)
	{
		printf("unable to open %s\n", edid_path);
		return 0;
	}
	/*** SP_TX_EDID_Read_Initial(); ***/
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_ADDR_7_0_REG, 0x50);
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_ADDR_15_8_REG, 0);
	// SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_ADDR_19_16_REG, &c);
	// SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_ADDR_19_16_REG, c & 0xf0);
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_ADDR_19_16_REG, 0);
	/******/

	/*** bEDID_Block = SP_TX_Get_EDID_Block(); ***/
	SP_TX_AuxOverI2C_Start();
	SP_TX_AUX_WR(0x7e);
	SP_TX_AuxOverI2C_ReStart();
	SP_TX_AUX_RD(0x05);
	SP_TX_AuxOverI2C_End();
	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_BUF_DATA_0_REG, &c);
	if (c == 0)
	{
		bEDID_Block = 0;
		// printf("EDID Block = 1\n");
	}
	else
	{
		bEDID_Block = 1;
		// printf("EDID Block = 2\n");
	}
	/******/

	bEDID_Block = 8 * (bEDID_Block + 1);

	debug_printf("			  0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F\n");

	for (i = 0; i < bEDID_Block; i++)
	{

		bReturn = SP_TX_AUX_EDIDRead_Byte(i * 16);
		if (bReturn == 0x02)
		{
			i = bEDID_Block;
		}
	}
	close(edid_fd);
	// clear the address only bit
	SP_TX_Read_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, &c);
	SP_TX_Write_Reg(SP_TX_PORT0_ADDR, SP_TX_AUX_CTRL_REG2, (c & ~0x02));

	SP_TX_RST_AUX();

	return 0;
}

const char *sopts = "d:a:r:w:b:u:hf";
static const struct option lopts[] = {
	{"dump_edid", required_argument, NULL, 'd'},
	{"addr", required_argument, NULL, 'a'},
	{"read_reg", required_argument, NULL, 'r'},
	{"write_reg", required_argument, NULL, 'w'},
	{"byte", required_argument, NULL, 'b'},
	{"bus", required_argument, NULL, 'u'},
	{"force_init", no_argument, NULL, 'f'},
	{"help", no_argument, NULL, 'h'},
	{0, 0, 0, 0}};
static void print_usage(const char *name)
{
	fprintf(stderr, "usage: %s options...\n", name);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "    -d --dump_edid      <path>       EDID store path\n");
	fprintf(stderr, "    -a --addr           <addr>       Port address\n");
	fprintf(stderr, "    -r --read_reg       <reg>        Read register\n");
	fprintf(stderr, "    -w --write_reg      <reg>        Write register\n");
	fprintf(stderr, "    -b --byte           <byte>       Byte to write\n");
	fprintf(stderr, "    -u --bus            <bus>        Bridge IC I2C bus\n");
	fprintf(stderr, "    -h --help                        Output usage message and exit.\n");
	fprintf(stderr, "    -f --force_init                  Force running dp init.\n");
}
int main(int argc, char **argv)
{
	int opt;
	uint8_t bus;
	uint8_t c;
	int i;
	uint32_t count = 0;
	bool force_init = false;
	uint8_t addr = 0;
	uint8_t reg = 0;
	uint8_t byte = 0;
	bool read_op = false;
	bool input_byte = false;
	bool plugged = false;
	bool initialized = false;

	while ((opt = getopt_long(argc, argv, sopts, lopts, NULL)) != EOF)
	{
		switch (opt)
		{
		case 'h':
			print_usage(argv[0]);
			exit(EXIT_SUCCESS);
		case 'd':
			edid_path = optarg;
			break;
		case 'a':
			addr = strtol(optarg, NULL, 16);
			break;
		case 'r':
			read_op = true;
		case 'w':
			reg = strtol(optarg, NULL, 16);
			break;
		case 'b':
			byte = strtol(optarg, NULL, 16);
			input_byte = true;
			break;
		case 'f':
			force_init = true;
			break;
		case 'u':
			bus = strtoul(optarg, NULL, 10);
			I2C_BUS_NUMBER = bus & 0xff;
			break;
		default:
			print_usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	dpinit_reset_control();

	// normal mode
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_POWERD_CTRL_REG, 0x00);

	// check vendor id
	SP_TX_Read_Reg(SLAVE_ADDR39, SP_TX_VND_IDH_REG, &c);
	if (c == SP_TX_VNDER_ID)
	{
		std::cerr << "ANX9807 Chip found" << std::endl;
	}
	else
	{
		std::cerr << "ANX9807 Chip not found" << std::endl;
		return -1;
	}

	// read/write register by request
	if (addr && reg)
	{
		if (read_op)
		{
			printf("read reg 0x%x @0x%x\n", reg, addr);
			SP_TX_Read_Reg(addr, reg, &c);
			printf("data = 0x%x\n", c);
		}
		else if (input_byte)
		{
			printf("write 0x%x to reg 0x%x @0x%x\n", byte, reg, addr);
			SP_TX_Write_Reg(addr, reg, byte);
		}
		return 0;
	}

	SP_TX_Read_Reg(SLAVE_ADDR39, SP_TX_VID_CTRL1_REG, &c);
	if (c == 0x84)
	{
		std::cerr << "ANX9807 Chip has been initialized" << std::endl;
		initialized = true;
		if (!edid_path && !force_init)
			return 0;
	}
	// software reset
	if (!initialized)
	{
		SP_TX_Read_Reg(SLAVE_ADDR39, SP_TX_RST_CTRL_REG, &c);
		SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_RST_CTRL_REG, c | SP_TX_RST_SW_RST);
		SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_RST_CTRL_REG, c & ~SP_TX_RST_SW_RST);
	}

	// force HPD and stream valid
	// SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_SYS_CTRL3_REG, 0x33);

	// check HPD status
	SP_TX_Read_Reg(SLAVE_ADDR38, SP_TX_SYS_CTRL3_REG, &c);
	if ((c & 0x40) == 0)
	{
		plugged = false;
		std::cerr << "Unplugged" << std::endl;
	}
	else
	{
		plugged = true;
		std::cerr << "Plugged" << std::endl;
	}

	// dump edid as request
	if (edid_path && plugged)
	{
		std::cerr << "Dump EDID" << std::endl;
		return dump_edid();
	}

	// enable HPD interrupt
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_COMMON_INT_MASK1, 0); // mask all int
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_COMMON_INT_MASK2, 0); // mask all int
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_COMMON_INT_MASK3, 0); // mask all int
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_COMMON_INT_MASK4, 7); // enable hpd int
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_INT_MASK, 0);		   // mask all int

	// clear interrupt status
	SP_TX_Read_Reg(SLAVE_ADDR39, SP_COMMON_INT_STATUS4, &c);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_COMMON_INT_STATUS4, c);

	// set edid address for AUX access
	SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_EXTRA_ADDR_REG, 0x50);

	// enable HPD polling
	SP_TX_Read_Reg(SLAVE_ADDR38, SP_TX_DEBUG_REG1, &c);
	SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_DEBUG_REG1, c | 0x20);

	SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_HDCP_CTRL, 0x00); // disable HDCP polling mode.
	// SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_HDCP_CTRL, 0x02);	//Enable HDCP polling mode.
	SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_LINK_DEBUG_REG, 0x30); // enable M value read out

	SP_TX_Read_Reg(SLAVE_ADDR38, SP_TX_HDCP_CONTROL_0_REG, &c);
	SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_HDCP_CONTROL_0_REG, c | 0x03); // set KSV valid

	SP_TX_Read_Reg(SLAVE_ADDR38, SP_TX_AUX_CTRL_REG2, &c);
	SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_AUX_CTRL_REG2, c | 0x08); // set double AUX output

	// reset AUX
	SP_TX_Read_Reg(SLAVE_ADDR39, SP_TX_RST_CTRL2_REG, &c);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_RST_CTRL2_REG, c | SP_TX_AUX_RST);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_RST_CTRL2_REG, c & (~SP_TX_AUX_RST));

	// Chip initialization
	SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_SYS_CTRL1_REG, 0x00);
	sleep(0.01);
	for (i = 0; i < 50; i++)
	{
		SP_TX_Read_Reg(SLAVE_ADDR38, SP_TX_SYS_CTRL1_REG, &c);
		SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_SYS_CTRL1_REG, c);
		SP_TX_Read_Reg(SLAVE_ADDR38, SP_TX_SYS_CTRL1_REG, &c);
		if ((c & SP_TX_SYS_CTRL1_DET_STA) == 0x04)
		{
			std::cerr << "clock is detected." << std::endl;
			break;
		}
		sleep(0.01);
	}
	// check whether clock is stable
	for (i = 0; i < 50; i++)
	{
		SP_TX_Read_Reg(SLAVE_ADDR38, SP_TX_SYS_CTRL2_REG, &c);
		SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_SYS_CTRL2_REG, c);
		SP_TX_Read_Reg(SLAVE_ADDR38, SP_TX_SYS_CTRL2_REG, &c);
		if ((c & SP_TX_SYS_CTRL2_CHA_STA) == 0x0)
		{
			std::cerr << "clock is stable." << std::endl;
			break;
		}
		sleep(0.01);
	}

	// VESA range, 8bits BPC, RGB
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_VID_CTRL2_REG, 0x10);
	// ANX9807 chip analog setting
	SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_PLL_CTRL_REG, 0x00); // UPDATE: FROM 0X07 TO 0X00
	// ANX chip analog setting
	//	SP_TX_Write_Reg(SLAVE_ADDR39, ANALOG_DEBUG_REG1, 0x70); 			  //UPDATE: FROM 0XF0 TO 0X70
	SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_LINK_DEBUG_REG, 0x30);

	// force HPD
	// SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_SYS_CTRL3_REG, 0x30);
	//  enable HPD detect
	SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_SYS_CTRL3_REG, 0);

	if (!plugged)
		return 0;

	// Select 2.7G
	SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_LINK_BW_SET_REG, 0x0a); // 2.7g:0x0a;1.62g:0x06
	// Select 2 lanes
	SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_LANE_COUNT_SET_REG, 0x02); // two lane
	SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_LINK_TRAINING_CTRL_REG, SP_TX_LINK_TRAINING_CTRL_EN);
	sleep(0.005);
	SP_TX_Read_Reg(SLAVE_ADDR38, SP_TX_LINK_TRAINING_CTRL_REG, &c);
	while ((c) != 0) // UPDATE: FROM 0X01 TO 0X80
	{
		sleep(0.005);
		count++;
		if (count > 100)
		{
			// OSAL_PRINTF("ANX9807 Link trainning fail...\n");
			std::cerr << "ANX9807 Link trainning fail...\n"
					  << std::endl;
			break;
		}
		SP_TX_Read_Reg(SLAVE_ADDR38, SP_TX_LINK_TRAINING_CTRL_REG, &c);
	}

	for (c = 0; c < 12; c++)
	{
		SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_VIDEO_BIT_CTRL_0_REG + c, 0x00 + c);
	}
	for (c = 0; c < 12; c++)
	{
		SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_VIDEO_BIT_CTRL_12_REG + c, 0x18 + c);
	}

	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_TOTAL_LINEL_REG, 0x26);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_TOTAL_LINEH_REG, 0x03);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_ACT_LINEL_REG, 0x00);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_ACT_LINEH_REG, 0x03);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_VF_PORCH_REG, 0x03);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_VSYNC_CFG_REG, 0x06);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_VB_PORCH_REG, 0x1d);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_TOTAL_PIXELL_REG, 0x40);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_TOTAL_PIXELH_REG, 0x05);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_ACT_PIXELL_REG, 0x00);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_ACT_PIXELH_REG, 0x04);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_HF_PORCHL_REG, 0x18);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_HF_PORCHH_REG, 0x00);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_HSYNC_CFGL_REG, 0x88);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_HSYNC_CFGH_REG, 0x00);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_HB_PORCHL_REG, 0xa0);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_HB_PORCHH_REG, 0x00);
	//	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_VID_CTRL10_REG, 0x03);//Bist mode
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_VID_CTRL10_REG, 0x00); // SYNC polarity
	// enable BIST. In normal mode, don't need to config this reg
	// SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_VID_CTRL4_REG, 0x09);//colorbar:08,graystep:09
	// enable video input, set DDR mode, the input DCLK should be 102.5MHz;
	// In normal mode, set this reg to 0x81, SDR mode, the input DCLK should be 205MHz
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_TX_VID_CTRL1_REG, 0x84); // Set ddr and post-edge trigger

	// force HPD and stream valid
	// SP_TX_Write_Reg(SLAVE_ADDR38, SP_TX_SYS_CTRL3_REG, 0x33);

	// clear interrupt status
	SP_TX_Read_Reg(SLAVE_ADDR39, SP_COMMON_INT_STATUS4, &c);
	SP_TX_Write_Reg(SLAVE_ADDR39, SP_COMMON_INT_STATUS4, c);

	std::cerr << "ANX9807 Chip finished initialization" << std::endl;

	return 0;
}
