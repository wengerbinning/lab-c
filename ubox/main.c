



{
	int rem;
	struct blob_attr *cur;

	blobmsg_for_each_attr( cur, tb[0], rem ) {
		blobmsg_format_json( cur, true );
	}
}