#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

/* Returneaza multimea cu cardinalul mai mic. */

set< int >& setmin( set< int >& A, set< int >& B )
{
    if( A.size( ) > B.size( ) )
        return A;
    
    return B;
}

/*
 * Calculeaza multimea starilor din care putem ajunge
 *  via simbolul c intr-o stare din X.
 * Returneaza: set< int > continand starile din care putem ajunge.
 */

set< int >& reverse( char c, set< int >& A, map< pair< int, char >, set< int > >& DAutoR )
{
    set< int >* rev = new set< int >;
    
    for( set< int >::iterator it = A.begin( ); it != A.end( ); ++it )
    {
        set< int >_dif = DAutoR[ make_pair( ( *it ), c ) ];
        
        for( set< int >::iterator ij = _dif.begin( ); ij != _dif.end( ); ++ij )
            rev->insert( ( *ij ) );
    }
    
    return *rev;
}

/*
 * Functia de rafinare( operator | )
 * Returneaza o pereche formata din intersectia si diferenta celor doua multimi.
 * Rafinarea este corecta daca ambele multimi sunt nevide.
 */

pair< set< int >, set< int > >& rafinare( set< int >& X, set< int >& Y )
{
    pair< set< int >, set< int > >* ret = new pair< set< int >, set< int > >;
    
    set_intersection( X.begin( ), X.end( ), Y.begin( ), Y.end( ), inserter( ret->first, ret->first.begin( ) ) );
    set_difference( X.begin( ), X.end( ), Y.begin( ), Y.end( ), inserter( ret->second, ret->second.begin( ) ) );
    
    return *ret;
}

int main( int argc, char* argv[ ] )
{
    int i, temp, stareCurenta, stareFinala,
    nrStariDFA, nrFinaleDFA, nrSimbDFA;
    
    char _character;
    
    FILE* f = fopen( "//Users//adi//Desktop//date.in", "r" );
    
    map< pair< int, char >, int > DAuto;
    map< pair< int, char >, set< int > > DAutoR;
    set< int > StariDFA;
    set< int > FinaleDFA;
    set< char > SimbDFA;
    vector< int > eFinala;
    
    fscanf( f, "%d %d %d", &nrStariDFA, &nrFinaleDFA, &nrSimbDFA );
    
    eFinala.resize( nrStariDFA + 1, 0 );
    
    for( i = 0; i < nrStariDFA; i++ )
        StariDFA.insert( i );
    
    for( i = 0; i < nrFinaleDFA; i++ )
    {
        fscanf( f, "%d", &temp );
        
        FinaleDFA.insert( temp );
        
        eFinala[ temp ] = 1;
    }
    
    while( fscanf( f, "%d %c %d", &stareCurenta, &_character, &stareFinala ) != EOF )
    {
        SimbDFA.insert( _character );
        
        DAuto[ make_pair( stareCurenta, _character ) ] = stareFinala; // Delta
        
        DAutoR[ make_pair( stareFinala, _character ) ].insert( stareCurenta ); // Delta^-1
    }
    
    fclose( f );
    
    /* Start Algoritm Hopcroft */
    
    vector< set< int > > P;
    vector< set< int > > W;
    
    set< int > aux;
    
    set_difference( StariDFA.begin( ), StariDFA.end( ), FinaleDFA.begin( ), FinaleDFA.end( ), inserter( aux, aux.begin( ) ) );
    
    P.push_back( aux );
    
    P.push_back( FinaleDFA );
    
    W.push_back( setmin( aux, FinaleDFA ) );
    
    while( !W.empty( ) )
    {
        set< int > A = W.back( ); W.pop_back( );
        
        for( set< char >::iterator c = SimbDFA.begin( ); c != SimbDFA.end( ); ++c )
        {
            for( i = 0; i < P.size( ); )
            {
                pair< set< int >, set< int > >& _Y = rafinare( ( P[ i ] ), reverse( ( *c ), A, DAutoR ) );
                
                if( _Y.first.size( ) > 0 && _Y.second.size( ) > 0 )
                {
                    aux = P[ i ];
                    
                    P.erase( P.begin( ) + i );
                    P.push_back( _Y.first );
                    P.push_back( _Y.second );
                    
                    vector< set< int > >::iterator it = find( W.begin( ), W.end( ), aux );
                    
                    if( it != W.end( ) )
                    {
                        W.erase( it );
                        W.push_back( _Y.first );
                        W.push_back( _Y.second );
                    }
                    else
                        W.push_back( setmin( _Y.first, _Y.second ) );
                }
                else
                    i++;
            }
        }
    }
    
    /* Codificare Minimal */
    
    map< pair< int, char >, int > MAuto;
    
    map< set< int >, int > sTOi;
    map< int, set< int > > iTOs;
    
    int nrStariM, nrFinaleM, nrSimbM;
    set< int > FinaleM;
    set< int > SimbM;
    
    sort( P.begin( ), P.end( ) );
    
    for( i = 0; i < P.size( ); i++ )
    {
        sTOi[ P[ i ] ] = i;
        iTOs[ i ] = P[ i ];
    }
    
    for( vector< set< int > >::iterator Pi = P.begin( ); Pi != P.end( ); ++Pi )
    {
        set< int >::iterator q = ( *Pi ).begin( );

        if( eFinala[ ( *q ) ] )
            FinaleM.insert( sTOi[ ( *Pi ) ] );
            
        for( set< char >::iterator c = SimbDFA.begin( ); c != SimbDFA.end( ); ++c )
        {
            if( DAuto.count( make_pair( ( *q ), ( *c ) ) ) > 0 )
            {
                int _q = DAuto[ make_pair( ( *q ), ( *c ) ) ];
                
                for( vector< set< int > >::iterator Pj = P.begin( ); Pj != P.end( ); ++Pj )
                {
                    if( find( ( *Pj ).begin( ), ( *Pj ).end( ), _q ) != ( *Pj ).end( ) )
                    {
                        MAuto[ make_pair( sTOi[ ( *Pi ) ], ( *c ) ) ] = sTOi[ ( *Pj ) ];
                        
                        SimbM.insert( ( *c ) );
                        
                        break;
                    }
                }
            }
            else
            {
                MAuto[ make_pair( sTOi[ ( *Pi ) ], ( *c ) ) ] = sTOi[ ( *Pi ) ];
                
                SimbM.insert( ( *c ) );
            }
        }
    }
    
    nrStariM = ( int )P.size( );
    nrFinaleM = ( int )FinaleM.size( );
    nrSimbM = ( int )SimbM.size( );
    
    cout << nrStariM << " " << nrFinaleM << " " << nrSimbM << endl;
    
    for( set< int >::iterator f = FinaleM.begin( ); f != FinaleM.end( ); ++f )
        cout << *f << " ";
    
    cout << endl;
    
    for( map< pair< int, char >, int >::iterator it = MAuto.begin( ); it != MAuto.end( ); ++it )
        cout << ( *it ).first.first << " " << ( *it ).first.second << " " << ( *it ).second << endl;
    
    return 0;
}

/* End */
