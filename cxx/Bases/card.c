#include <stdbool.h>
#include <stdio.h>

enum Suit
{
  Trèfle = 1,
  Diamand,
  Coeur,
  Pique,
};

enum Face
{
  Un = 1,
  Deux,
  Trois,
  Quatre,
  Cinq,
  Six,
  Sept,
  Huit,
  Neuf,
  Dix,

  Valet,
  Reine,
  Roi,
  As
};

struct Card
{
  enum Suit suit;
  int suit_value;

  enum Face face;
  int face_value;

  bool is_wild;
};

struct Hand
{
  int cards_dealt;
  struct Card c1, c2, c3, c4, c5;
};

struct Hand
add_card(struct Hand old_hand, struct Card card);

void
print_hand(struct Hand hand);
void
print_hand2(struct Hand hand);
void
print_hand3(struct Hand hand);
void
print_card(struct Card card);

int
main(void)
{
  struct Hand hand = { 0 };

  struct Card c1 = { Pique, (int)Pique, Dix, (int)Dix, false };
  struct Card c2 = { Coeur, (int)Coeur, Reine, (int)Reine, false };
  struct Card c3 = { Diamand, (int)Diamand, Cinq, (int)Cinq, false };
  struct Card c4 = { Trèfle, (int)Trèfle, As, (int)As, false };
  struct Card c5 = { Coeur, (int)Coeur, Valet, (int)Valet, false };
  struct Card c6 = { Trèfle, (int)Trèfle, Deux, (int)Deux, false };

  hand = add_card(hand, c1);
  hand = add_card(hand, c2);
  hand = add_card(hand, c3);
  hand = add_card(hand, c4);
  hand = add_card(hand, c5);
  hand = add_card(hand, c6);

  print_hand(hand);
  printf("\n");
  print_hand2(hand);
  printf("\n");
  print_hand3(hand);

  return 0;
}

struct Hand
add_card(struct Hand old_hand, struct Card card)
{
  struct Hand newHand = old_hand;

  switch (newHand.cards_dealt) {
    case 0:
      newHand.c1 = card;
      newHand.cards_dealt++;
      break;
    case 1:
      newHand.c2 = card;
      newHand.cards_dealt++;
      break;
    case 2:
      newHand.c3 = card;
      newHand.cards_dealt++;
      break;
    case 3:
      newHand.c4 = card;
      newHand.cards_dealt++;
      break;
    case 4:
      newHand.c5 = card;
      newHand.cards_dealt++;
      break;
    default:
      // ERREUR: ignorer la nouvelle carte.
      newHand = old_hand;
      break;
  }

  return newHand;
}

void
print_hand(struct Hand hand)
{
  for (int i = 1; i < hand.cards_dealt + 1; i++) { // 1..5
    struct Card card;

    switch (i) {
      case 1:
        card = hand.c1;
        break;
      case 2:
        card = hand.c2;
        break;
      case 3:
        card = hand.c3;
        break;
      case 4:
        card = hand.c4;
        break;
      case 5:
        card = hand.c5;
        break;
      default:
        return;
        break;
    }

    print_card(card);
  }
}

void
print_hand2(struct Hand hand)
{
  int dealt = hand.cards_dealt;
  if (dealt == 0)
    return;

  print_card(hand.c1);
  if (dealt == 1)
    return;

  print_card(hand.c2);
  if (dealt == 2)
    return;

  print_card(hand.c3);
  if (dealt == 3)
    return;

  print_card(hand.c4);
  if (dealt == 4)
    return;

  print_card(hand.c5);
  return;
}

void
print_hand3(struct Hand hand)
{
  for (int i = 1; i < hand.cards_dealt + 1; i++) { // 1..5
    switch (i) {
      case 1:
        print_card(hand.c1);
        break;
      case 2:
        print_card(hand.c2);
        break;
      case 3:
        print_card(hand.c3);
        break;
      case 4:
        print_card(hand.c4);
        break;
      case 5:
        print_card(hand.c5);
        break;
      default:
        return;
        break;
    }
  }
}

void
print_card(struct Card card)
{
  switch (card.face) {
    case Deux:
      printf("    2 ");
      break;
    case Trois:
      printf("    3 ");
      break;
    case Quatre:
      printf("    4 ");
      break;
    case Cinq:
      printf("    5 ");
      break;
    case Six:
      printf("    6 ");
      break;
    case Sept:
      printf("    7 ");
      break;
    case Huit:
      printf("    8 ");
      break;
    case Neuf:
      printf("    9 ");
      break;
    case Dix:
      printf("   10 ");
      break;
    case Valet:
      printf("Valet ");
      break;
    case Reine:
      printf("Reine ");
      break;
    case Roi:
      printf("  Roi ");
      break;
    case As:
      printf("   As ");
      break;
    default:
      printf("  ??? ");
      break;
  }
  switch (card.suit) {
    case Pique:
      printf("de piques\n");
      break;
    case Coeur:
      printf("de coeurs\n");
      break;
    case Diamand:
      printf("de diamands\n");
      break;
    case Trèfle:
      printf("de trèfles\n");
      break;
    default:
      printf("de ???\n");
      break;
  }
}