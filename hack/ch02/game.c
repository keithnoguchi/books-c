/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#define DATAFILE "/var/game.data"

/* User model */
struct user {
	int uid;
	int credits;
	int highscore;
	char name[100];
	int (*current_game)(struct user *);
};

static int pick_a_number(struct user *player);
static int dealer_no_match(struct user *player);
static int find_the_ace(struct user *player);
static void play(struct user *player);
static void show_highscore(struct user *player);
static int input_name(struct user *player);
static int input_number(int *number);
static void update_player_data(struct user *player);
static struct user *init(const char *const progname);

int main(int argc, const char *const argv[])
{
	const char *progname = argv[0];
	struct user *player;
	int ret, choice, last_game;

	player = init(progname);
	if (!player)
		exit(EXIT_FAILURE);

	choice = last_game = -1;
	while (choice != 7) {
		printf("-=[ Game of Chance Menu ]=-\n");
		printf("1 - Play the Pick a Number game\n");
		printf("2 - Play the No Match Dealer game\n");
		printf("3 - Play the Find the Ace game\n");
		printf("4 - View current high score\n");
		printf("5 - Change your user name\n");
		printf("6 - Reset your account at 100 credits\n");
		printf("7 - Quit\n");
		printf("[Name: %s]\n", player->name);
		printf("[You have %u credits] --> ", player->credits);
		ret = input_number(&choice);
		if (ret == -1)
			break;

		ret = 0;
		if (choice < 1 || choice > 7)
			printf("\nThe number %d is an invalid selection.\n\n",
			       choice);
		else if (choice < 4) {
			/* update the current_game pointer */
			if (choice != last_game) {
				last_game = choice;
				switch (choice) {
				case 1:
					player->current_game = pick_a_number;
					break;
				case 2:
					player->current_game = dealer_no_match;
					break;
				case 3:
					player->current_game = find_the_ace;
					break;
				}
			}
			play(player);
		} else if (choice == 4)
			show_highscore(player);
		else if (choice == 5) {
			printf("Enter your name: ");
			ret = input_name(player);
			if (ret == -1)
				break;
			update_player_data(player);
		} else if (choice == 6) {
			player->credits = 100;
			update_player_data(player);
			printf("\nYour account has been reset with %d credits.\n\n",
			       player->credits);
		}
		if (ret == -1)
			break;
	}
	printf("\nThanks for playing! Bye.\n");
	free(player);
	exit(EXIT_SUCCESS);
}

/* get name from stdin */
static int input_name(struct user *player)
{
	size_t n = sizeof(player->name);
	char *p, c = '\n';
	int i = 0;

	/* flush any leftover */
	while (c == '\n')
		scanf("%c", &c);

	p = player->name;
	while (c != '\n') {
		if (++i >= n)
			break;
		*p = c;
		scanf("%c", &c);
		p++;
	}
	*p = '\0';
	return 0;
}

/* get number from stdin */
static int input_number(int *number)
{
	int ret = scanf("%d", number);
	if (ret == EOF)
		return -1;
	else if (ret == 0) {
		char dummy = '\n';
		while (dummy == '\n')
			scanf("%c", &dummy);
		return -1;
	}
	return 0;
}

static int take_wager(struct user *player, int previous_wager)
{
	int ret, wager = -1, total_wager;

	printf("How many of your %d credits would you like to wager? ",
	       player->credits);

	ret = input_number(&wager);
	if (ret == -1)
		return -1;
	if (wager < 1) {
		printf("Nice try, but you must wager a positive number!\n");
		return -1;
	}
	total_wager = previous_wager + wager;
	if (total_wager > player->credits) {
		printf("Your total wager of %d is more than you have!\n",
		       total_wager);
		printf("You only have %d available credits, try again.\n\n",
		       player->credits);
		return -1;
	}
	return wager;
}

static void print_cards(const char *const msg, char *cards, int user_pick)
{
	printf("\n\t*** %s *** \n", msg);
	printf("      \t._.\t._.\t._.\n");
	printf("Cards:\t|%c|\t|%c|\t|%c|\n\t",
		cards[0], cards[1], cards[2]);
	if (user_pick == -1)
		printf(" 1 \t 2 \t 3\n");
	else {
		int i;
		for (i = 0; i < user_pick; i++)
			printf("\t");
		printf(" ^-- your pick\n");
	}
}

/* Pick a Number game */
static int pick_a_number(struct user *player)
{
	int pick, winning_number;

	printf("\n##### Pick a Number #####\n");
	printf("This game costs 10 credits to play.  Simply pick a number\n");
	printf("between 1 and 20, and if you pick the winning number, you\n");
	printf("will win the jackpot of 100 credits!\n\n");
	winning_number = rand()%20+1;
	if (player->credits < 10) {
		printf("You only have %d credits.  That's not enough to play!\n\n",
		       player->credits);
		return -1;
	}
	do {
		int ret;

		printf("pick a number between 1 and 20: ");
		ret = input_number(&pick);
		if (ret == -1)
			break;
	} while (pick < 1 || pick > 20);
	player->credits -= 10;
	if (pick != winning_number)
		return 0;
	return 2;
}

/* No Match Dealer game */
static int dealer_no_match(struct user *player)
{
	int i, counts[100], wager = -1, match = -1;

	printf("\n::::: No Match Dealer :::::\n");
	printf("In this game, you can wager up to all of your credits.\n");
	printf("The dealer will deal out 16 random numbers between 0 and 99.\n");
	printf("If there are no matches among them, you double your money!\n\n");

	while (wager == -1)
		wager = take_wager(player, 0);

	printf("\t\t:: Dealing out 16 random numbers :::\n");
	memset(counts, 0, sizeof(counts));
	for (i = 0; i < 16; i++) {
		int number = rand() % 100;
		printf("%2d\t", number);
		if (i%8 == 7)
			printf("\n");
		if (counts[number]++)
			match = number;
	}
	if (match != -1) {
		printf("The dealer matched the number %d!\n", match);
		printf("You lose %d credits.\n", wager);
		player->credits -= wager;
		return 0;
	}
	printf("There were no matches!  You win %d credits!\n", wager);
	player->credits += wager;
	return 1;
}

/* Find the Ace game */
static int find_the_ace(struct user *player)
{
	char cards[] = {'X', 'X', 'X'};
	int i, ace, pick = -1, wager_one, wager_two;

	ace = rand() % 3;

	printf("******* Find the Ace ********\n");
	printf("In this game, you can wager up to all of your credits.\n");
	printf("Three cards will be dealt out, two queens and one ace.\n");
	printf("If you find the ace, you will win your wager.\n");
	printf("After choosing a card, one of the queens will be revealed.\n");
	printf("At this point, you may either select a different card or\n");
	printf("increase your wager.\n\n");

	wager_one = wager_two = -1;
	while (wager_one == -1)
		wager_one = take_wager(player, 0);

	print_cards("Dealing cards", cards, -1);
	do {
		int ret;
		printf("Select a card: 1, 2, or 3  ");
		ret = input_number(&pick);
		if (ret == -1)
			break;
	} while (pick < 1 || pick > 3);
	printf("ace = %d, pick = %d\n", ace, pick);
	pick--;
	/* Find the non-selected queen */
	for (i = 0; i == ace || i == pick; i++)
		;
	cards[i] = 'Q';
	print_cards("Revealing a queen", cards, pick);

	while (true) {
		char choice = '\n';
		printf("Would you like to:\n[c]hange your pick "
		       "or[i]ncrease your wager? ");
		while (choice == '\n')
			scanf("%c", &choice);
		if (choice == 'i') {
			wager_two = -1;
			while (wager_two == -1)
				wager_two = take_wager(player, wager_one);
			break;
		} else if (choice == 'c') {
			for (i = 0; i == pick || cards[i] == 'Q'; i++)
				;
			pick = i;
			printf("Your card pick has been changed to card %d\n",
			       pick+1);
			break;
		}
	}

	for (i = 0; i < 3; i++) {
		if (ace == i)
			cards[i] = 'A';
		else
			cards[i] = 'Q';
	}
	print_cards("End result", cards, pick);

	if (pick != ace) {
		printf("You have lost %d credits from your first wager\n",
		       wager_one);
		player->credits -= wager_one;
		if (wager_two != -1) {
			printf("and an additional %d credits from your "
			       "second wager!\n", wager_two);
			player->credits -= wager_two;
		}
		return 0;
	}
	printf("You have won %d credits from your first wager\n", wager_one);
	player->credits += wager_one;
	if (wager_two != -1) {
		printf("and an additional %d credits from your "
		       "second wager!\n", wager_two);
		player->credits += wager_two;
	}
	return 1;
}

static void jackpot(struct user *player)
{
	printf("*+*+*+*+*+* JACKPOT *+*+*+*+*+*+*\n");
	printf("You have won the jackpot of 100 credits\n");
	player->credits += 100;
}

static void play(struct user *player)
{
	bool again = true;

	while (again) {
		char selection = '\n';

		if (player->credits == 0) {
			printf("\nYou don't have any credits!\n");
			printf("Please refill it.\n\n");
			return;
		}
		printf("\n[DEBUG] current_game pointer = %p\n",
		       player->current_game);
		switch (player->current_game(player)) {
		case -1:
			return;
		case 0:
			printf("Sorry, you didn't win.\n");
			update_player_data(player);
			break;
		case 1:
			printf("You win!\n");
			update_player_data(player);
			break;
		case 2:
			jackpot(player);
			break;
		}
		printf("\nYou now have %u credits\n", player->credits);
		printf("Would you like to play again? (y/n) ");
		while (selection == '\n') {
			int ret = scanf("%c", &selection);
			if (ret != 1)
				break;
		}
		if (selection != 'y')
			again = false;
	}
}

static int get_player_data(struct user *player)
{
	const char *const db = DATAFILE;
	int fd, uid, len;

	uid = getuid();

	/* find the user record */
	fd = open(db, O_RDONLY);
	if (fd == -1)
		goto err;
	len = read(fd, player, sizeof(*player));
	if (len == -1)
		goto err;
	while (player->uid != uid && len > 0)
		len = read(fd, player, sizeof(*player));
	if (len == -1)
		goto err;
	if (len != sizeof(*player)) {
		printf("%s: no user record for %u\n", __FUNCTION__, uid);
		return -1;
	}
	return 0;
err:
	fprintf(stderr, "%s: %s\n", __FUNCTION__, strerror(errno));
	close(fd);
	return -1;
}

static int register_new_player(struct user *player)
{
	const char *db = DATAFILE;
	int ret, fd = -1;

	printf("-=-={ New Player Registration }=-=-\n");
	printf("Enter your name: ");

	ret = input_name(player);
	if (ret == -1)
		goto err;
	player->uid = getuid();
	player->highscore = player->credits = 100;
	player->current_game = NULL;

	fd = open(db, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
	if (fd == -1)
		goto err;
	ret = write(fd, player, sizeof(*player));
	if (ret == -1)
		goto err;
	if (ret != sizeof(*player)) {
		printf("%s: partial write, exiting", __FUNCTION__);
		goto err;
	}
	close(fd);

	printf("\nWelcome to the Game of Chance %s.\n", player->name);
	printf("You have been given %u credits.\n", player->credits);
	return 0;
err:
	if (fd != -1)
		close(fd);
	if (ret == -1)
		fprintf(stderr, "%s: %s\n", __FUNCTION__, strerror(errno));
	return -1;
}

static void update_player_data(struct user *player)
{
	const char *const db  = DATAFILE;
	int fd, ret = -1;

	fd = open(db, O_RDWR);
	if (fd == -1)
		goto err;

	/* update the user data */
	while (1) {
		off_t offset = sizeof(struct user) - sizeof(int);
		int uid;

		ret = read(fd, &uid, sizeof(uid));
		if (ret == -1)
			goto err;
		if (uid == player->uid)
			break;
		ret = lseek(fd, offset, SEEK_CUR);
		if (ret == -1)
			goto err;
	}
	if (player->credits > player->highscore)
		player->highscore = player->credits;
	ret = write(fd, &player->credits, sizeof(player->credits));
	if (ret == -1)
		goto err;
	ret = write(fd, &player->highscore, sizeof(player->highscore));
	if (ret == -1)
		goto err;
	ret = write(fd, player->name, sizeof(player->name));
err:
	if (ret == -1)
		fprintf(stderr, "%s: %s\n", __FUNCTION__, strerror(errno));
	if (fd != -1)
		close(fd);
}

static void show_highscore(struct user *player)
{
	const char *const db = DATAFILE;
	char top_name[BUFSIZ];
	int top_score = -1;
	struct user entry;
	int fd;

	printf("\n=====================| HIGH SCORE |===================\n");
	fd = open(db, O_RDONLY);
	if (fd == -1)
		goto err;
	while (read(fd, &entry, sizeof(entry)) > 0) {
		if (entry.highscore > top_score) {
			top_score = entry.highscore;
			strncpy(top_name, entry.name, sizeof(top_name));
		}
	}
	close(fd);
	if (top_score > player->highscore)
		printf("%s has the high score of %d\n", top_name, top_score);
	else
		printf("You currently have the high score of %d credits!\n",
		       player->highscore);
	printf("========================================================\n");
err:
	fprintf(stderr, "%s: %s\n", __FUNCTION__, strerror(errno));
	if (fd != -1)
		close(fd);
}

static struct user *init(const char *const progname)
{
	struct user *player;
	int ret;

	srand(time(NULL));

	player = malloc(sizeof(struct user));
	if (!player)
		return NULL;

	/* find the player record */
	ret = get_player_data(player);
	if (ret == 0)
		return player;

	/* register new player */
	ret = register_new_player(player);
	if (ret == 0)
		return player;

	/* failed to initialize player */
	free(player);
	return NULL;
}
