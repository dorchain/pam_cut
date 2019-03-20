/*
 * pam_modules that cuts off supllied passwords after given count
 * takes 1 paramater: cut=<number>
 * 
 * Use case is e.g. cutting off after static part when user entered <static_part><OTP>
 * 
 * Only Rudimentary error handling
 *
 * License: GPL 2+
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAM_SM_AUTH
#include <security/pam_modules.h>


PAM_EXTERN int pam_sm_authenticate(pam_handle_t	*pamh, 	 
	int flags, 	 
 	int argc, 	 
 	const char **argv)
{
	int i,len;
	int retval;
	const char *password = NULL;
	char *cutpw = NULL;

	int cut=44;

	for (i = 0; i < argc; i++) {
		if (strncmp (argv[i], "cut=", 4) == 0)
			sscanf(argv[i], "cut=%d", &cut);
        /* XXX: TODO; bark unknown args etc */
	}
	if (cut < 0)
		cut = 0;

	if ((retval = pam_get_item (pamh, PAM_AUTHTOK, (const void **) &password)) != PAM_SUCCESS)
		return retval;
	if ( password == NULL )
		return PAM_SUCCESS; /* Nothing to cut */
	len = strlen(password);
	if (len < cut)
		return PAM_SUCCESS; /* Nothing to cut */
	cutpw = strdup(password);
	if (!cutpw) 
		return PAM_BUF_ERR;
	cutpw[len-cut] = '\0';
	retval = pam_set_item (pamh, PAM_AUTHTOK, cutpw);
	memset(cutpw, 0, len);
	free(cutpw);
	return retval;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, 	 
	int flags, 	 
 	int argc, 	 
 	const char **argv)
{
	return (PAM_SUCCESS);
}
