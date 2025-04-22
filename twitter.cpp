#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <queue>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

class Twitter
{
private:
    map<int, list<pair<int, int>>> tweet; // userId -> lista de tweets
    map<int, set<int>> followers;         // userId -> conjunto de usuarios seguidos
    int timestamp;                        // marca de tiempo para ordenar los tweets

public:
    Twitter() : timestamp(0) // Inicializamos la marca de tiempo a 0
    {
    }

    void postTweet(int userId, int tweetId) // Publica un tweet
    {
        tweet[userId].push_front({timestamp++, tweetId}); // Agregamos el tweet a la lista de tweets del usuario
    }

    vector<int> getNewsFeed(int userId) // Obtiene el feed de noticias del usuario
    {
        // Se puede usar un max heap para obtener los 10 tweets más recientes.
        priority_queue<pair<int, int>> recent_tweets;
        for (const auto &val : tweet[userId]) // Agregamos los tweets del usuario al max heap
        {
            recent_tweets.push(val);
        }

        // También debemos obtener los tweets de los usuarios seguidos.
        for (const auto &f : followers[userId])
        {
            for (const auto &ft : tweet[f]) // Agregamos los tweets de los usuarios seguidos al max heap
            {
                recent_tweets.push(ft); // Agregamos el tweet al max heap
            }
        }

        // Ahora necesitamos encontrar los 10 tweets más recientes.
        vector<int> tweets;
        int tweet_count = 0;
        while (!recent_tweets.empty() && tweet_count < 10) // Mientras haya tweets y no hayamos alcanzado el límite de 10
        {
            tweets.push_back(recent_tweets.top().second); // Obtenemos el tweet más reciente
            recent_tweets.pop();                          // Lo eliminamos del max heap
            ++tweet_count;                                // Aumentamos el contador de tweets
        }
        return tweets;
    }

    void follow(int followerId, int followeeId) // Seguir a un usuario
    {
        followers[followerId].insert(followeeId); // Agregamos el usuario seguido al conjunto de seguidores
    }

    void unfollow(int followerId, int followeeId) // Dejar de seguir a un usuario
    {
        followers[followerId].erase(followeeId); // Eliminamos el usuario seguido del conjunto de seguidores
    }
};

int main()
{
    Twitter twitter;

    // Usuarios publican tweets
    twitter.postTweet(1, 100); // Usuario 1 publica tweet 100
    twitter.postTweet(2, 200); // Usuario 2 publica tweet 200
    twitter.postTweet(1, 101); // Usuario 1 publica otro tweet

    // Usuario 1 sigue a usuario 2
    twitter.follow(1, 2);

    // Mostrar feed de usuario 1 (debería incluir tweets 200, 101, 100)
    vector<int> feed = twitter.getNewsFeed(1);
    cout << "Feed de usuario 1:" << endl;
    for (int tweetId : feed)
    {
        cout << tweetId << " ";
    }
    cout << endl;

    // Usuario 1 deja de seguir a usuario 2
    twitter.unfollow(1, 2);

    // Mostrar feed actualizado (ahora solo tweets del usuario 1)
    feed = twitter.getNewsFeed(1);
    cout << "Feed de usuario 1 después de unfollow:" << endl;
    for (int tweetId : feed)
    {
        cout << tweetId << " ";
    }
    cout << endl;

    return 0;
}