# include <Siv3D.hpp>
# include "asc/TCPString.hpp"
# include "ColorRect.hpp"

GUI CreateGUI(const Vec2& scale, const Vec2& offset, int port)
{
	GUI gui(GUIStyle::Default);

	gui.setTitle(L"Setting");
	gui.addln(GUIText::Create(L"Scale"));
	gui.add(GUIText::Create(L"("));
	gui.add(L"scaleX", GUITextField::Create(5));
	gui.add(GUIText::Create(L", "));
	gui.add(L"scaleY", GUITextField::Create(5));
	gui.addln(GUIText::Create(L")"));
	gui.addln(GUIText::Create(L"Offset"));
	gui.add(GUIText::Create(L"("));
	gui.add(L"offsetX", GUITextField::Create(5));
	gui.add(GUIText::Create(L", "));
	gui.add(L"offsetY", GUITextField::Create(5));
	gui.addln(GUIText::Create(L")"));
	gui.addln(GUIText::Create(L"Network"));
	gui.addln(L"state", GUIText::Create(L"State: Stop"));
	gui.add(GUIText::Create(L"Port"));
	gui.addln(L"port", GUITextField::Create(5));
	gui.addln(L"action", GUIButton::Create(L"Run"));

	gui.textField(L"scaleX").setText(ToString(scale.x));
	gui.textField(L"scaleY").setText(ToString(scale.y));
	gui.textField(L"offsetX").setText(ToString(offset.x));
	gui.textField(L"offsetY").setText(ToString(offset.y));
	gui.textField(L"port").setText(ToString(port));

	return gui;
}

GUI CreateGUI()
{
	return CreateGUI(Vec2::One, Vec2::Zero, 50000);
}

void Main()
{
	Window::SetTitle(L"Augmented Building Blocks");
	Window::SetStyle(WindowStyle::Sizeable);
	Window::Maximize();

	auto gui = CreateGUI();

	asc::TCPStringServer server;
	bool isAccept = false;

	while (System::Update())
	{
		if (server.hasError() && isAccept)
		{
			const auto code = static_cast<int>(server.getError());

			server.disconnect();
			isAccept = false;
			gui.button(L"action").text = L"Run";
			gui.text(L"state").text = Format(L"State: Error (CODE: ", code, L")");

			LOG_ERROR(Format(L"SERVER: ErrorCode ", code));
		}

		if (isAccept)
		{
			if (gui.button(L"action").pushed)
			{
				server.isConnected() ? server.disconnect() : server.cancelAccept();
				isAccept = false;
				gui.button(L"action").text = L"Run";
				gui.text(L"state").text = L"State: Stop";
			}

			if (server.isConnected())
			{
				gui.text(L"state").text = L"Connected";
			}
		}
		else
		{
			if (gui.button(L"action").pushed)
			{
				server.startAccept(static_cast<uint16>(Parse<int>(gui.textField(L"port").text)));
				isAccept = true;
				gui.text(L"state").text = Format(L"State: Running at ", gui.textField(L"port").text);
				gui.button(L"action").text = L"Stop";
			}
		}

		if (Input::KeySpace.clicked)
		{
			gui.show(!gui.style.visible);
			Cursor::SetStyle(gui.style.visible ? CursorStyle::Default : CursorStyle::None);
		}
	}
}
