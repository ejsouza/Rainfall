class N {
	private:
		int nb;
		char buffer[104];

	N(unsigned int numb) {
		nb = numb;	
	}

	public:
		int operator+(N& n) {
			return nb + n.nb;
		}
			int operator-(N& n) {
			return nb - n.nb;
		}
		void *setAnnotation(char *str) {
			unsigned int len = strlen(str);
			return (memcpy(buffer, str, len));
		}
};

int		main(int argc, char **argv) {
	if (argc < 2) {
		exit(1);
	}

	N *a = new N(5);
	N *b = new N(6);

	a->setAnnotation(argv[1]);
	*a + *b;
	return (0);
}