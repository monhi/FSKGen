#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/*
 * Default values for 1200bps V.23
 */
#define SAMPRATE  22100.0
#define SAMPLES   22100
#define ZERO_FREQ 2200.0
#define ZERO_RAD  (2.0*M_PI*ZERO_FREQ)
#define ONE_FREQ  1200.0
#define ONE_RAD   (2.0*M_PI*ONE_FREQ)
#define BITRATE   1200

/*
 * CID Data:
 *      30 bytes of alternating bits (200 ms)
 *      21 bytes of mark (1's)       (140 ms) (spec says 130 +/- 25ms)
 *      CID Data
 */
unsigned char CIDData[64] = {
        0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
        0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
        0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0x04, 0x12, 0x30, 0x39, 0x33,
        0x30, 0x31, 0x32, 0x32, 0x34, 0x36, 0x30, 0x39,
        0x35, 0x35, 0x35, 0x31, 0x32, 0x31, 0x32, 0x51,
};

#define NRBITS  512

void print_help(void)
{
        printf("Usage: mksin [-b bitrate] [-s samples] [-r rate]\n");
        printf("             [-o freq] [-z freq] [-a] [-d]\n");

}

/*
 * get_next_bit() - Reteive the next bit of CID data from the CID data
 * array.
 */
int get_next_bit(void)
{
        static int byte_counter = 0;
        static int bit_counter = 7;
        int bit;

        bit = (CIDData[byte_counter] & (1 << bit_counter)) >> bit_counter;
        if (bit_counter == 0) {
                bit_counter = 7;
                byte_counter++;
        } else  bit_counter--;

        return bit;

}

int main (int argc, char *argv[])
{
        int i, t = 0;
        int count = 0, residual = 0;
        int br;
        int result;
        int bit;
        int bits = 0;
        double x;
        double freq, oldfreq;

        int disp = 0;
        int samples = SAMPLES;
        int rate = SAMPRATE;
        int bitrate = BITRATE;
        double one_freq = ONE_RAD;
        double zero_freq = ZERO_RAD;
        int verbose = 0;
        int opt;

        while ((opt = getopt(argc,argv,"ab:dho:r:s:vz:")) != -1)
        {
                switch (opt)
                {
                        case 'a':
                                disp = 0;
                                break;
                        case 'b':
                                bitrate = atoi(optarg);
                                break;
                        case 'd':
                                disp = 1;
                                break;
                        case 'h':
                                print_help();
                                exit(0);
                        case 'o':
                                one_freq = 2.0 * M_PI * (double)atoi(optarg);
                                break;
                        case 'r':
                                rate = atoi(optarg);
                                break;
                        case 's':
                                samples = atoi(optarg);
                                break;
                        case 'v':
                                verbose++;
                                break;
                        case 'z':
                                zero_freq = 2.0 * M_PI * (double)atoi(optarg);
                                break;
                }
        }

        bit = get_next_bit();
        if (bit)
                freq = one_freq;
        else    freq = zero_freq;

        for (i = 0; bits < NRBITS; i++)
        {
                x = (((float)t/rate) * freq);
                result = 127 + (127 * sin(x));
                t++;
                count++;
                br = (rate / count) - residual;
                if (br <= bitrate)
                {
					if (verbose)
							printf ("switch: %d %d", count, t);
					residual = bitrate - br;
					count = 0;

					if (bit != get_next_bit()) {
							oldfreq = freq;
							if (bit == 0) {
									bit = 1;
									freq = one_freq;
							} else {
									bit = 0;
									freq = zero_freq;
							}
							if (verbose)
									printf (" flip ");

							t = (t * (oldfreq / freq)) + 1;
					}

					if (verbose)
							printf(" %d\n", t);
					bits++;
                }

                if (disp)
                        printf ("%d %d\n", i, result);
                else
                        printf ("%c",result);
        }

        return 0;
}