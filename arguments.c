/**
 * CLI Argument Parser (both *nix and win32 formats)
 *
 * @author Gareth Stones <gatty.nz@gmail.com>
 * @license GNU General Public License
 *
 * @bug -k=v && --key=value accepts x{1,} = chars
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Win32 macros
 */

#ifdef WIN32
#define snprintf(a,b,c,d) sprintf_s(a, b, c, d);
#endif

/**
 * Function prototype's
 */

void argumentShort (char *key, char *value);
void argumentLong (char *key, char *value);
#ifdef WIN32
void argumentWin32 (char *key, char *value);
#endif
void argumentUsageAndHelp (const char *programName);
char *argumentGetKey (char *key);
char *argumentGetValue (char *key);

/**
 * Send argv[i] && argv[i+1] off to there correct parser's
 */

int main (int argc, char *argv[]) {

   if (argc == 1)
      argumentUsageAndHelp(argv[0]);

   int i, size;

   for (i = 1; i < argc; i++) {
      size = (sizeof(char*) * strlen(argv[i]));
      char *current = (char*) malloc(size + 1);
      snprintf(current, size + 1, "%s", argv[i]);
      if (*(current) == '-' && *(current+1) == '-')
         argumentLong(argv[i], argv[i+1]);
      else if (*(current) == '-' && *(current+1) != '-')
         argumentShort(argv[i], argv[i+1]);
#ifdef WIN32
      else if (*(current) == '/')
         argumentWin32(argv[i], argv[i+1]);
#endif
   }

   return 0;
}

/**
 * CLI usage and help
 */

void argumentUsageAndHelp (const char *programName) {

   fprintf(stderr, "usage: %s (arguments)\n", programName);

   exit(-1);
}

#ifdef WIN32

/**
 * Parse Windows only CLI argument's
 */

void argumentWin32 (char *key, char *value) {

   if (strlen(key) <= 1) {
      fprintf(stderr, "Invalid CLI argument length (win32).\n");
      exit(-1);
   }

   /**
    * Settings that do not require usage of value argument
    */

   if (strncmp(key, "/?", 2) == 0 && strlen(key) == 2 && value == NULL)
      printf("%s argument with no value, setting to default\n", key);

   if (value == NULL)
      return;

   /**
    * Settings that require usage of value argument
    */

   if (strncmp(key, "/example", 8) == 0 && strlen(key) == 8)
      printf("%s %s\n", key, value);
}

#endif

/**
 * Parse short CLI argument's
 *
 * @accept -k
 *         -k value
 *         -k=value
 */

void argumentShort (char *key, char *value) {

   if (strlen(key) <= 1) {
      fprintf(stderr, "Invalid CLI argument length (short).\n");
      exit(-1);
   }

   /**
    * Settings that do not require usage of value argument
    */

   if (strncmp(key, "-e", 2) == 0 && strlen(key) == 2 && value == NULL)
      printf("%s argument with no value, setting to default\n", key);

   /**
    * Check if we're dealing with -k=value if value is NULL
    */

   if (value == NULL) {
      if (strchr(key, '=') != NULL) {
         value = argumentGetValue(key);
         key = argumentGetKey(key);
      }
      else return;
   }

   if (strncmp(key, "-e", 2) == 0 && strlen(key) == 2)
      printf("%s %s\n", key, value);
}

/**
 * Parse long CLI argument's
 *
 * @accept --key
 *         --key value
 *         --key=value
 */

void argumentLong (char *key, char *value) {

   if (strlen(key) <= 2) {
      fprintf(stderr, "Invalid CLI argument length (long)\n");
      exit(-1);
   }

   /**
    * Settings that do not require usage of value argument
    */

   if (strncmp(key, "--example", 9) == 0 && strlen(key) == 9 && value == NULL)
      printf("%s with no value, setting to default\n", key);

   /**
    * Check if we're dealing with --key=value if value is NULL
    */

   if (value == NULL) {
      if (strchr(key, '=') != NULL) {
         value = argumentGetValue(key);
         key = argumentGetKey(key);
      }
      else
         return;
   }

   if (strncmp(key, "--example", 9) == 0 && strlen(key) == 9)
      printf("%s %s\n", key, value);
}

/**
 * Extract key from -k=value or --key=value syntax
 */

char *argumentGetKey (char *key) {

   return strtok(key, "=");
}

/**
 * Extract value from -k=value or --key=value syntax
 */

char *argumentGetValue (char *key) {

   char *value;

   value = strtok(key, "=");
   value = strtok(NULL, "=");

   return value;
}
