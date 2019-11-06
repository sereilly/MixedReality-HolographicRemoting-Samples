#pragma once
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Networking.Sockets.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Xaml.Navigation.h>
#include <sstream>

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Xaml::Navigation;

class UdpServer
{
private:
	winrt::Windows::Networking::Sockets::DatagramSocket m_serverDatagramSocket;
public:
	IAsyncAction StartServer(const hstring& port);
	IAsyncAction Send(const winrt::array_view<const uint8_t>& buffer);
	void StopStream();
	void StopServer();
private:
	IAsyncAction ServerDatagramSocket_MessageReceived(winrt::Windows::Networking::Sockets::DatagramSocket sender, winrt::Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs args);
	DataWriter m_dataWriter = nullptr;
	hstring m_port;
};
