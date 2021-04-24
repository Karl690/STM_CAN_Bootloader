#awk '{print strtonum("0x" $1)}'
BEGIN {}
{
	if ($1 == "08000000") {
		addr2 = strtonum("0x" $1);
		name = $2;
	}
	else {
		addr1 = addr2;
		addr2 = strtonum("0x" $1);
		size = addr2 - addr1;
		printf("%12d  %s\n", size, name);
		name = $2;
	}
}
END{}

